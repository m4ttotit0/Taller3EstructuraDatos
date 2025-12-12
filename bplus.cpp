#include <iostream>
#include "bplus.h"

//contador externo
extern long contAcceso;

NodoBPlusBase::NodoBPlusBase(int _orden, bool _es_hoja)
    : claves(new int[_orden]), cuenta(0),
      orden(_orden), es_hoja(_es_hoja) {}

NodoBPlusBase::~NodoBPlusBase() {
    delete [] claves;
}
//getters
bool NodoBPlusBase::esHoja() const { return es_hoja; }
int  NodoBPlusBase::getCuenta() const { return cuenta; }
int  NodoBPlusBase::getClave(int i) const { return claves[i]; }
void NodoBPlusBase::setClave(int i, int v) { claves[i] = v; }
void NodoBPlusBase::setCuenta(int c) { cuenta = c; }

NodoBInterno::NodoBInterno(int _orden)
    : NodoBPlusBase(_orden, false),
      punteros(new NodoBPlusBase*[_orden + 1]) {
    for (int i = 0; i < _orden + 1; ++i) punteros[i] = nullptr;
}

NodoBInterno::~NodoBInterno() {
    delete [] punteros;
}

int NodoBInterno::buscar_siguiente(int clave) {
    int i = 0;
    while (i < cuenta && clave >= claves[i]) ++i;
    return i;
}

NodoBPlusBase* NodoBInterno::getPuntero(int i) { return punteros[i]; }

void NodoBInterno::setPuntero(int i, NodoBPlusBase* p) { punteros[i] = p; }

NodoBHoja::NodoBHoja(int _orden)
    : NodoBPlusBase(_orden, true),
      datos(new NodoGrafo*[_orden]),
      siguiente_hoja(nullptr) {
    for (int i = 0; i < _orden; ++i) datos[i] = nullptr;
}

NodoBHoja::~NodoBHoja() {
    delete [] datos;
}

NodoBHoja* NodoBHoja::getSiguiente() { return siguiente_hoja; }

void NodoBHoja::setSiguiente(NodoBHoja* s) { siguiente_hoja = s; }

ArbolBPlus::ArbolBPlus(int _orden)
    : raiz(nullptr), orden(_orden) {}

NodoBHoja* ArbolBPlus::insertar_en_hoja(NodoBHoja* hoja, int clave,
                                        NodoGrafo* valor,
                                        int& outClavePromovida) {
    int i = 0;
    while (i < hoja->cuenta && clave > hoja->claves[i]) ++i;
    for (int j = hoja->cuenta; j > i; --j) {
        hoja->claves[j] = hoja->claves[j - 1];
        hoja->datos[j]  = hoja->datos[j - 1];
    }
    hoja->claves[i] = clave;
    hoja->datos[i]  = valor;
    hoja->cuenta++;

    if (hoja->cuenta < orden) return nullptr;

    int mitad = hoja->cuenta / 2;
    NodoBHoja* nueva_hoja = new NodoBHoja(orden);

    int j2 = 0;
    for (int j = mitad; j < hoja->cuenta; ++j, ++j2) {
        nueva_hoja->claves[j2] = hoja->claves[j];
        nueva_hoja->datos[j2]  = hoja->datos[j];
    }
    nueva_hoja->cuenta = j2;
    hoja->cuenta = mitad;

    nueva_hoja->siguiente_hoja = hoja->siguiente_hoja;
    hoja->siguiente_hoja = nueva_hoja;

    outClavePromovida = nueva_hoja->claves[0];
    return nueva_hoja;
}

NodoBPlusBase* ArbolBPlus::insertar_rec(NodoBPlusBase* nodo, int clave,
                                        NodoGrafo* valor,
                                        int& outClavePromovida) {
    if (nodo->esHoja()) {
        return insertar_en_hoja((NodoBHoja*)nodo, clave, valor,
                                outClavePromovida);
    }

    NodoBInterno* interno = (NodoBInterno*)nodo;
    int idx = interno->buscar_siguiente(clave);
    contAcceso++;
    NodoBPlusBase* hijo = interno->getPuntero(idx);

    int clave_hijo_prom;
    NodoBPlusBase* nuevo_hijo =
        insertar_rec(hijo, clave, valor, clave_hijo_prom);

    if (!nuevo_hijo) return nullptr;

    int i = 0;
    while (i < interno->cuenta && clave_hijo_prom > interno->claves[i]) ++i;

    for (int j = interno->cuenta; j > i; --j) {
        interno->claves[j] = interno->claves[j - 1];
        interno->setPuntero(j + 1, interno->getPuntero(j));
    }
    interno->claves[i] = clave_hijo_prom;
    interno->setPuntero(i + 1, nuevo_hijo);
    interno->cuenta++;

    if (interno->cuenta < orden) return nullptr;

    int mitad = interno->cuenta / 2;

    NodoBInterno* nuevo_interno = new NodoBInterno(orden);
    outClavePromovida = interno->claves[mitad];

    int j2 = 0;
    for (int j = mitad + 1; j < interno->cuenta; ++j, ++j2) {
        nuevo_interno->claves[j2] = interno->claves[j];
    }
    for (int j = mitad + 1; j <= interno->cuenta; ++j) {
        nuevo_interno->setPuntero(j2, interno->getPuntero(j));
        ++j2;
    }
    nuevo_interno->cuenta = interno->cuenta - mitad - 1;

    interno->cuenta = mitad;

    return nuevo_interno;
}

void ArbolBPlus::insertar(int clave, NodoGrafo* valor) {
    if (!raiz) {
        NodoBHoja* hoja = new NodoBHoja(orden);
        hoja->claves[0] = clave;
        hoja->datos[0]  = valor;
        hoja->cuenta    = 1;
        raiz = hoja;
        return;
    }

    int clave_prom;
    NodoBPlusBase* nuevo = insertar_rec(raiz, clave, valor, clave_prom);
    if (nuevo) {
        NodoBInterno* nueva_raiz = new NodoBInterno(orden);
        nueva_raiz->claves[0] = clave_prom;
        nueva_raiz->setPuntero(0, raiz);
        nueva_raiz->setPuntero(1, nuevo);
        nueva_raiz->cuenta = 1;
        raiz = nueva_raiz;
    }
}

NodoGrafo* ArbolBPlus::buscar(int clave) {
    NodoBPlusBase* actual = raiz;
    if (!actual) return nullptr;

    while (actual && !actual->esHoja()) {
        NodoBInterno* interno = (NodoBInterno*)actual;
        int idx = interno->buscar_siguiente(clave);
        contAcceso++;
        actual = interno->getPuntero(idx);
    }

    if (!actual) return nullptr;
    NodoBHoja* hoja = (NodoBHoja*)actual;
    contAcceso++;

    for (int i = 0; i < hoja->cuenta; ++i) {
        if (hoja->claves[i] == clave)
            return hoja->datos[i];
    }
    return nullptr;
}

void ArbolBPlus::eliminar(int clave) {
    NodoBPlusBase* actual = raiz;
    if (!actual) return;

    while (actual && !actual->esHoja()) {
        NodoBInterno* interno = (NodoBInterno*)actual;
        int idx = interno->buscar_siguiente(clave);
        actual = interno->getPuntero(idx);
    }
    if (!actual) return;

    NodoBHoja* hoja = (NodoBHoja*)actual;
    contAcceso++;

    for (int i = 0; i < hoja->cuenta; ++i) {
        if (hoja->claves[i] == clave) {
            for (int j = i + 1; j < hoja->cuenta; ++j) {
                hoja->claves[j - 1] = hoja->claves[j];
                hoja->datos[j - 1]  = hoja->datos[j];
            }
            hoja->cuenta--;
            break;
        }
    }
}
