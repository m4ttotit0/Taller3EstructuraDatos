#include <iostream>
#include "system.h"

using namespace std;

int nextId = 1;
long contAcceso = 0;
ArbolBPlus arbolIndice;
int numRute = 0;

static int construir_rutas_rec(NodoGrafo *nodo, string *rutas, int idx_inicial, const string &sufijo)
{
    if (nodo->get_num_padres() == 0)
    {
        rutas[idx_inicial] = "/" + nodo->get_nombre() + sufijo;
        return 1;
    }

    int total = 0;
    int *padres = nodo->lista_padres();
    for (int i = 0; i < nodo->get_num_padres(); ++i)
    {
        NodoGrafo *p = buscar_nodo_grafo(padres[i]);
        if (!p)
            continue;
        string nuevo_sufijo = "/" + nodo->get_nombre() + sufijo;
        total += construir_rutas_rec(p, rutas, idx_inicial + total, nuevo_sufijo);
    }
    return total;
}

static int sumar_tamano_rec(NodoGrafo *nodo)
{
    if (!nodo)
        return 0;
    if (!nodo->es_directorio())
    {
        return ((NodoArchivo *)nodo)->get_tamano();
    }

    NodoDirectorio *dir = (NodoDirectorio *)nodo;
    int total = 0;
    int *hijos = dir->lista_hijos();
    for (int i = 0; i < dir->get_num_hijos(); ++i)
    {
        NodoGrafo *hijo = buscar_nodo_grafo(hijos[i]);
        total += sumar_tamano_rec(hijo);
    }
    return total;
}

void insertar_nodo_grafo(int clave, NodoGrafo *nodo_grafo)
{
    arbolIndice.insertar(clave, nodo_grafo);
}

NodoGrafo *buscar_nodo_grafo(int clave)
{
    contAcceso = 0;
    NodoGrafo *res = arbolIndice.buscar(clave);
    if (res)
        cout << "Accesos: " << contAcceso << " (ID: " << clave << ")\n";
    return res;
}

void crear_nodo(int id_padre)
{
    NodoGrafo *padre = buscar_nodo_grafo(id_padre);
    if (!padre || !padre->es_directorio())
    {
        cout << "Error: Directorio invalido.\n";
        return;
    }

    NodoDirectorio *dir_padre = (NodoDirectorio *)padre;

    int tipo_nodo;
    cout << "Crear (0 = archivo, 1 = directorio): ";
    cin >> tipo_nodo;

    string nombre;
    cout << "Nombre: ";
    cin >> nombre;

    int nuevo_id = nextId++;

    NodoGrafo *nuevo = nullptr;
    if (tipo_nodo == 1)
    {
        nuevo = new NodoDirectorio(nuevo_id, nombre);
    }
    else
    {
        int tam, tipo_archivo;
        cout << "Tamaño: ";
        cin >> tam;
        cout << "Tipo (0:Imagen,1:Documento,2:Exel,3:Video,4:Zip): ";
        cin >> tipo_archivo;
        nuevo = new NodoArchivo(nuevo_id, nombre, tam, tipo_archivo);
    }

    nuevo->agregar_padre(id_padre);
    dir_padre->agregar_hijo(nuevo_id);

    insertar_nodo_grafo(nuevo_id, nuevo);
    cout << "Nodo creado exitosamente con ID " << nuevo_id << "\n";
}

void eliminar_archivo(int id_archivo, int id_directorio_padre)
{
    NodoGrafo *nodo_arch = buscar_nodo_grafo(id_archivo);
    if (!nodo_arch || nodo_arch->es_directorio())
    {
        cout << "Error: ID no es un archivo valido.\n";
        return;
    }

    NodoGrafo *padre = buscar_nodo_grafo(id_directorio_padre);
    if (!padre || !padre->es_directorio())
    {
        cout << "Error: Directorio invalido.\n";
        return;
    }

    NodoDirectorio *dir_padre = (NodoDirectorio *)padre;
    dir_padre->eliminar_hijo(id_archivo);
    nodo_arch->eliminar_padre(id_directorio_padre);

    if (nodo_arch->get_num_padres() == 0)
    {
        arbolIndice.eliminar(id_archivo);
        delete nodo_arch;
        cout << "Archivo eliminado del sistema.\n";
    }
    else
    {
        cout << "Referencia eliminada. Archivo aun existe en otros directorios.\n";
    }
}

void listar_contenido(int id_directorio)
{
    NodoGrafo *nodo = buscar_nodo_grafo(id_directorio);
    if (!nodo || !nodo->es_directorio())
    {
        cout << "Error: No es un directorio valido.\n";
        return;
    }

    NodoDirectorio *dir = (NodoDirectorio *)nodo;
    int *hijos = dir->lista_hijos();
    int n = dir->get_num_hijos();

    cout << "\n--- Contenido de " << dir->get_nombre() << " (ID=" << dir->get_id() << ") ---\n";
    for (int i = 0; i < n; ++i)
    {
        NodoGrafo *hijo = buscar_nodo_grafo(hijos[i]);
        if (!hijo)
            continue;
        cout << "  -> " << hijo->get_nombre()
             << " [ID=" << hijo->get_id() << "] "
             << (hijo->es_directorio() ? "(DIR)" : "(FILE)") << "\n";
    }
    cout << "--------------------------------------\n";
}

string *obtener_rutas_completas(int id_archivo)
{
    NodoGrafo *nodo = buscar_nodo_grafo(id_archivo);
    if (!nodo)
    {
        cout << "Error: ID invalido.\n";
        numRute = 0;
        return nullptr;
    }

    string *rutas = new string[64];
    int generadas = construir_rutas_rec(nodo, rutas, 0, "");
    numRute = generadas;

    return rutas;
}

int calcular_espacio_ocupado(int id_directorio)
{
    NodoGrafo *nodo = buscar_nodo_grafo(id_directorio);
    if (!nodo || !nodo->es_directorio())
    {
        cout << "Error: ID no es directorio.\n";
        return 0;
    }
    int total = sumar_tamano_rec(nodo);
    cout << "Espacio total ocupado: " << total << " bytes/unidades.\n";
    return total;
}

