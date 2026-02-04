/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correos: alu0101655659@ull.edu.es y alu0101639185@ull.edu.es
 * Fecha: 04/02/2026
 * Main program.
 * Allows generating or loading the distribution, intuitively 
 * selecting variables of interest and conditioned variables, 
 * and automatically computing the conditional distribution.
 */

#include <fstream>
#include "inferencia_condicional.h"
#include "tools.h"

/**
 * Function that prints a message explaining the purpose of the program.
 */
void PrintProgramPurpose() {
  std::cout << "Este programa permite generar o cargar una distribución de probabilidades" << std::endl;
std::cout << "y calcula automáticamente distribuciones condicionales basadas en las variables seleccionadas." << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
  PrintProgramPurpose();
  const std::string kHelptext = "Introduce un archivo CSV con la distribución o ningún parámetro para generarla aleatoriamente";
  Usage(argc, argv, 2, kHelptext);
  
  Inference inference;

  if (argc > 1) {
    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
      throw std::runtime_error("Error: No se pudo abrir el archivo " + std::string(argv[1]));
    }
    input_file >> inference;
    std::cout << "Distribución cargada desde el archivo: " << argv[1] << std::endl;
    input_file.close();
  } else {
    inference = Inference();
    std::cout << "Distribución generada aleatoriamente." << std::endl;
  } 
  
  inference.askVariables();
  return 0;
}