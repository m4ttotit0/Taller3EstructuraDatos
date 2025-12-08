#ifndef system.h
#define system.h

#include <string>
#include "nodo_grafo.h"
#include "bplus.h"

extern int next;
extern long access;
extern ArbolBPlus index;
extern int numRute;

void insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo);
NodoGrafo* buscar_nodo_grafo(int clave);
void crear_nodo(int id_padre);
void eliminar_archivo(int id_archivo, int id_directorio_padre);
void listar_contenido(int id_directorio);
string* obtener_rutas_completas(int id_archivo);
int calcular_espacio_ocupado(int id_directorio);
