#include "NodoGrafo.h"

NodoGrafo::NodoGrafo(int _id, const std::string& _nombre)
    : id(_id), nombre(_nombre),
      lista_padres(nullptr), num_padres(0), cap_padres(0) {}

NodoGrafo::~NodoGrafo() {
    delete [] padres;
}

void NodoGrafo::asegurar_capacidad_padres() {
    if (num_padres < cap_padres) return;
    int nueva_cap = (cap_padres == 0 ? 2 : cap_padres * 2);
    int* nuevo = new int[nueva_cap];
    for (int i = 0; i < num_padres; ++i) nuevo[i] = padres[i];
    delete [] padres;
    padres = nuevo;
    cap_padres = nueva_cap;
}

int NodoGrafo::get_id() const { return id; }

const std::string& NodoGrafo::get_nombre() const { return nombre; }

bool NodoGrafo::es_directorio() { return false; }

int* NodoGrafo::lista_padres() { return padres; }

int NodoGrafo::get_num_padres() const { return num_padres; }

void NodoGrafo::agregar_padre(int id_padre) {
    asegurar_capacidad_padres();
    lista_padres[num_padres++] = id_padre;
}

void NodoGrafo::eliminar_padre(int id_padre) {
    for (int i = 0; i < num_padres; ++i) {
        if (lista_padres[i] == id_padre) {
            for (int j = i + 1; j < num_padres; ++j)
                lista_padres[j - 1] = lista_padres[j];
            --num_padres;
            break;
        }
    }
}

NodoDirectorio::NodoDirectorio(int _id, const std::string& _nombre)
    : NodoGrafo(_id, _nombre),
      hijos(nullptr), num_hijos(0), cap_hijos(0) {}

NodoDirectorio::~NodoDirectorio() {
    delete [] hijos;
}

void NodoDirectorio::asegurar_capacidad_hijos() {
    if (num_hijos < cap_hijos) return;
    int nueva_cap = (cap_hijos == 0 ? 2 : cap_hijos * 2);
    int* nuevo = new int[nueva_cap];
    for (int i = 0; i < num_hijos; ++i) nuevo[i] = hijos[i];
    delete [] hijos;
    hijos = nuevo;
    cap_hijos = nueva_cap;
}

bool NodoDirectorio::es_directorio() { return true; }

int* NodoDirectorio::lista_hijos() { return hijos; }

int NodoDirectorio::get_num_hijos() const { return num_hijos; }

void NodoDirectorio::agregar_hijo(int id_hijo) {
    asegurar_capacidad_hijos();
    hijos[num_hijos++] = id_hijo;
}

void NodoDirectorio::eliminar_hijo(int id_hijo) {
    for (int i = 0; i < num_hijos; ++i) {
        if (hijos[i] == id_hijo) {
            for (int j = i + 1; j < num_hijos; ++j)
                hijos[j - 1] = hijos[j];
            --num_hijos;
            break;
        }
    }
}

NodoArchivo::NodoArchivo(int _id, const std::string& _nombre,
                         int _tam, int _tipo)
    : NodoGrafo(_id, _nombre), tamano(_tam), tipo(_tipo) {}

bool NodoArchivo::es_directorio() { return false; }

int NodoArchivo::get_tamano() const { return tamano; }

int NodoArchivo::get_tipo() const { return tipo; }
