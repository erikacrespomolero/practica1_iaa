/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia  Curso: 2o
 * Práctica 2: Cadenas y lenguajes
 * Autor: Érika Crespo Molero
 * Correo: alu0101639185@ull.edu.es
 * Fecha: 18/09/2024
 * Archivo tools.cc
 * Contiene la función Usage para tomar los parámetros correctos y ayudar al usuario a ejecutar el programa.
 * 18/09/2024 - Creación (primera versión) del código
 */

#include <iostream>
#include <cstdlib>
#include <sstream>

/**
 * The function checks that the user has entered the correct number of
 * parameters when starting the program.
 *
 * @param[in] argc Number of parameters entered by the user
 * @param[in] argv Vector containing pointers to the different
 * parameters entered by the user
 * @return If the user enters --help, a brief message will be returned
 * containing basic instructions for the program and how to execute it.
 * @return Otherwise, nothing is returned
 */
void Usage(const int argc, char* argv[], int size_argc, const std::string& kHelpText) {
  if (argc > 1 && std::string(argv[1]) == "--help") {
    std::cout << kHelpText << std::endl;
    exit(EXIT_SUCCESS);
  }
  if (argc != size_argc) {
    std::cout << argv[0] << ": Falta que introduzca uno o más parámetros." << std::endl;
    std::cout << "Pruebe " << argv[0] << " --help para más información" << std::endl;
    exit(EXIT_SUCCESS);
  }
}