/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correo: alu0101639185@ull.edu.es
 */

#include "inferencia_condicional.h"

void PrintProgramPurpose() {
  std::cout << "Este programa tiene como entrada un fichero de entrada y otro de salida y realiza " << std::endl;
  std::cout << "y resuelve el laberinto dinámico con A*." << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    PrintProgramPurpose();
    const std::string kHelptext = "Introduce un archivo de input y un archivo de output así: ./maze archivo_entrada archivo_salida";
    Usage(argc, argv, 3, kHelptext);
    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
      throw std::runtime_error("Error: No se pudo abrir el archivo " + std::string(argv[1]));
    }
    Maze maze_to_read;
    input_file >> maze_to_read;
    input_file.close();
    std::ofstream output_file(argv[2]);
    if (!output_file.is_open()) {
      throw std::runtime_error("Error: No se pudo crear/abrir el archivo " + std::string(argv[2]));
    }
    output_file << "Mapa inicial del laberinto: " << '\n';
    output_file << maze_to_read;
    bool sol = false;
    maze_to_read.ExecuteDynamicAStar(output_file);
    output_file.close();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Error desconocido ocurrido." << std::endl;
    return 1;
  }
  return 0;
}