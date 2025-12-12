#pragma once
#include " NodoGrafo .h"

const int ORDEN_BPLUS = 4;

class NodoBPlusBase {
protected:
    int* claves;
    int cuenta;
    int orden;
    bool es_hoja;

public:
    NodoBPlusBase(int _orden, bool _es_hoja);
    virtual ~NodoBPlusBase();

    bool esHoja() const;
    int getCuenta() const;
    int getClave(int i) const;
    void setClave(int i, int v);
    void setCuenta(int c);

    friend class ArbolBPlus;
};

class NodoBInterno : public NodoBPlusBase {
    NodoBPlusBase** punteros;

public:
    NodoBInterno(int _orden);
    ~NodoBInterno();

    int buscar_siguiente(int clave);

    NodoBPlusBase* getPuntero(int i);
    void setPuntero(int i, NodoBPlusBase* p);

    friend class ArbolBPlus;
};

class NodoBHoja : public NodoBPlusBase {
    NodoGrafo** datos;
    NodoBHoja* siguiente_hoja;

public:
    NodoBHoja(int _orden);
    ~NodoBHoja();

    NodoBHoja* getSiguiente();
    void setSiguiente(NodoBHoja* s);

    friend class ArbolBPlus;
};

class ArbolBPlus {
    NodoBPlusBase* raiz;
    int orden;

    NodoBHoja* insertar_en_hoja(NodoBHoja* hoja, int clave,
                                NodoGrafo* valor, int& outClavePromovida);
    NodoBPlusBase* insertar_rec(NodoBPlusBase* nodo, int clave,
                                NodoGrafo* valor, int& outClavePromovida);

public:
    ArbolBPlus(int _orden = ORDEN_BPLUS);

    void insertar(int clave, NodoGrafo* valor);
    NodoGrafo* buscar(int clave);
    void eliminar(int clave);
};
