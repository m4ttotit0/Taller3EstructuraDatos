#pragma once
#include <string>

class NodoGrafo {
protected:
    int id;
    std::string nombre;

    int* lista_padres;
    int num_padres;
    int cap_padres;

    void asegurar_capacidad_padres();

public:
    NodoGrafo(int _id, const std::string& _nombre);
    virtual ~NodoGrafo();

    int get_id() const;
    const std::string& get_nombre() const;

    virtual bool es_directorio();

    int* lista_padres();
    int get_num_padres() const;

    void agregar_padre(int id_padre);
    void eliminar_padre(int id_padre);
};

class NodoDirectorio : public NodoGrafo {
    int* hijos;
    int num_hijos;
    int cap_hijos;

    void asegurar_capacidad_hijos();

public:
    NodoDirectorio(int _id, const std::string& _nombre);
    ~NodoDirectorio();

    bool es_directorio() override;

    int* lista_hijos();
    int get_num_hijos() const;

    void agregar_hijo(int id_hijo);
    void eliminar_hijo(int id_hijo);
};

class NodoArchivo : public NodoGrafo {
    int tamano;
    int tipo; // 0: Imagen, 1: Documento, 2: Ejecutable, 3: Video, 4: Comprimido

public:
    NodoArchivo(int _id, const std::string& _nombre, int _tam, int _tipo);

    bool es_directorio() override;

    int get_tamano() const;
    int get_tipo() const;
};
