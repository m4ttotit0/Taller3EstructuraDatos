#include <iostream>
#include "system.h"

void menu() {
  std::cout << "\n Sistema de Archivos \n";
  std::cout << "1. Crear archivo\n";
  std::cout << "2. Eliminar archivo\n";
  std::cout << "3. Listar contenido de directorio\n";
  std::cout << "4. Buscar archivo por id\n";
  std::cout << "5. Obtener rutas completas\n";
  std::cout << "6. Calcular espacio ocupado\n";
  std::cout << "9. Salir\n";
  std::cout << "Ingrese su opcion: ";
} 

int main() {
  // Crear raíz
    int id_root = nextId++;
    NodoDirectorio* root = new NodoDirectorio(id_root, "root");
    insertar_nodo_grafo(id_root, root);

    int opcion;
    do {
        mostrar_menu();
        std::cin >> opcion;

        if (opcion == 1) {
            //Llamar función crear archivo
        } else if (opcion == 2) {
          //eliminar archivo
        } else if (opcion == 3) {
          //lista contenido
        } else if (opcion == 4) {
          //buscar arch por id
        } else if (opcion == 5) {
          //rutas completas
        } else if (option == 6){
          //calcular espacio
	      }
	} while(opcion != 9);

	return 0;
}

