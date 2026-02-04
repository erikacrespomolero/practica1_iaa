/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correos: alu0101655659@ull.edu.es y alu0101639185@ull.edu.es
 * Fecha: 04/02/2026
 * File tools.cc
 * Contains the Usage function to handle the correct parameters 
 * and assist the user in running the program.
 */

#include <iostream>
#include <cstdlib>
#include <sstream>

/**
 * The function checks that the user has entered the correct number of
 * parameters when starting the program.
 *
 * @param[in] argc Number of parameters entered by the user.
 * @param[in] argv Vector containing pointers to the different
 * parameters entered by the user.
 * @return If the user enters --help, a brief message will be returned
 * containing basic instructions for the program and how to execute it.
 * @return Otherwise, nothing is returned.
 */
void Usage(const int argc, char* argv[], int size_argc, const std::string& kHelpText) {
  if (argc > 1 && std::string(argv[1]) == "--help") {
    std::cout << kHelpText << std::endl;
    exit(EXIT_SUCCESS);
  }
  if (argc > size_argc) {
    std::cout << argv[0] << ": Wrong number of parameters introduced." << std::endl;
    std::cout << "Try " << argv[0] << " --help for more information" << std::endl;
    exit(EXIT_SUCCESS);
  }
}