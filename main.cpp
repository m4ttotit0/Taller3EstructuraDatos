#include <iostream>
#include "system.h"

using namespace std;

void menu() {
  std::cout << "\n Sistema de Archivos \n";
  std::cout << "1. Crear archivo\n";
  std::cout << "2. Eliminar archivo\n";
  std::cout << "3. Listar contenido de directorio\n";
  std::cout << "4. Buscar archivo por id\n";
  std::cout << "5. Obtener rutas completas\n";
  std::cout << "6. Calcular espacio ocupado\n";
  std::cout << "9. Salir\n";
  std::cout << "Ingrese su opcion: \n";
} 

int main()
{
  int id_root = nextId++;
  NodoDirectorio *root = new NodoDirectorio(id_root, "root");
  insertar_nodo_grafo(id_root, root);

  cout << "Sistema montado. ID: " << id_root << "\n";

  int opcion;
  do
  {
    menu();
    if (!(cin >> opcion))
    {
      cin.clear();
      cin.ignore(10000, '\n');
      opcion = 0;
    }

    switch (opcion)
    {
    case 1:
    {
      int id_padre;
      cout << "ID Directorio Padre: ";
      cin >> id_padre;
      crear_nodo(id_padre);
      break;
    }
    case 2:
    {
      int id_arch, id_padre;
      cout << "ID Archivo: ";
      cin >> id_arch;
      cout << "ID Directorio Padre: ";
      cin >> id_padre;
      eliminar_archivo(id_arch, id_padre);
      break;
    }
    case 3:
    {
      int id_dir;
      cout << "ID Directorio: ";
      cin >> id_dir;
      listar_contenido(id_dir);
      break;
    }
    case 4:
    {
      int id;
      cout << "ID a buscar: ";
      cin >> id;
      NodoGrafo *n = buscar_nodo_grafo(id);
      if (!n)
        cout << "No encontrado.\n";
      else
        cout << "Encontrado\n";
      break;
    }
    case 5:
    {
      int id;
      cout << "ID Archivo: ";
      cin >> id;
      string *rutas = obtener_rutas_completas(id);
      if (rutas)
      {
        cout << "Rutas encontradas (" << numRute << "):\n";
        for (int i = 0; i < numRute; ++i)
          cout << rutas[i] << "\n";
        delete[] rutas;
      }
      break;
    }
    case 6:
    {
      int id;
      cout << "ID Directorio: ";
      cin >> id;
      calcular_espacio_ocupado(id);
      break;
    }
    case 9:
      cout << "Saliendo...\n";
      break;
    default:
      cout << "Opcion invalida.\n";
    }
  } while (opcion != 9);

  return 0;
}
