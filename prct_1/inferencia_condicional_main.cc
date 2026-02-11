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
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>
#include "inferencia_condicional.h"
#include "tools.h"

/**
 * Function that prints a message explaining the purpose of the program.
 */
void PrintProgramPurpose() {
  std::cout << "==========================================================" << std::endl;
  std::cout << "    INFERENCIA CONDICIONAL EN DISTRIBUCIONES DISCRETAS    " << std::endl;
  std::cout << "              VARIABLES BINARIAS                          " << std::endl;
  std::cout << "==========================================================" << std::endl;
  std::cout << std::endl;
  std::cout << "Este programa permite:" << std::endl;
  std::cout << "1. Generar o cargar una distribución de probabilidad conjunta" << std::endl;
  std::cout << "2. Seleccionar variables condicionadas y de interés" << std::endl;
  std::cout << "3. Calcular distribuciones condicionales mediante marginalización" << std::endl;
  std::cout << "4. Guardar resultados detallados en archivos (para distribuciones grandes)" << std::endl;
  std::cout << std::endl;
}

/**
 * @brief Saves execution data to CSV file.
 * @param[in] num_variables Total number of variables in the distribution.
 * @param[in] num_cond_vars Number of conditioned variables.
 * @param[in] num_interest_vars Number of interest variables.
 * @param[in] execution_time Execution time in microseconds.
 * @param[in] conditional_dist_size Size of the conditional distribution.
 */
void SaveExecutionToCSV(int num_variables, int num_cond_vars, int num_interest_vars, 
                       long long execution_time, int conditional_dist_size) {
  std::ofstream output_file("output.csv", std::ios::app);
  output_file.seekp(0, std::ios::end);
  if (output_file.tellp() == 0) {
    output_file << "num_variables,num_cond_vars,num_interest_vars,conditional_dist_size,execution_time_us\n";
  }
  output_file << num_variables << ","
              << num_cond_vars << ","
              << num_interest_vars << ","
              << conditional_dist_size << ","
              << execution_time << "\n";
  output_file.close();
}

int main(int argc, char* argv[]) {
  PrintProgramPurpose();
  std::string input_filename = "";
  std::string output_filename = "";
  bool use_output_file = false;
  const std::string kHelptext = "Introduce un archivo CSV con la distribución o ningún parámetro para generarla aleatoriamente";
  Usage(argc, argv, 2, kHelptext);
  if (argc > 1) {
    input_filename = argv[1];
  }
  Inference inference;
  if (!input_filename.empty()) {
    std::ifstream input_file(input_filename);
    if (!input_file.is_open()) {
      std::cerr << "Error: No se pudo abrir el archivo " << input_filename << std::endl;
      return 1;
    }
    input_file >> inference;
    input_file.close();
  } else {
    int num_variables;
    std::cout << "\nIngrese el número de variables para generar: ";
    std::cin >> num_variables;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (num_variables <= 0 || num_variables > 32) {
      std::cerr << "Error: El número de variables debe estar entre 1 y 32." << std::endl;
      return 1;
    }
    inference = Inference(num_variables);
  }
  // For large distributions, automatically use output file
  int total_configs = static_cast<int>(std::pow(2, inference.number_of_variables()));
  if (total_configs > 1024 && !use_output_file) {  
    output_filename = "distribucion_detallada.txt";
    use_output_file = true;
    std::cout << "\n  Distribución grande detectada (" << total_configs << " configuraciones)." << std::endl;
    std::cout << "   La salida se guardará en: " << output_filename << std::endl;
  }
  // Show joint distribution (to file if specified)
  if (use_output_file) {
    inference.PrintJointDistribution(output_filename);
  } else {
    inference.PrintJointDistribution(""); 
  }
  inference.AskVariables();
  // Calculate conditional distribution and measure time
  auto start_time = std::chrono::high_resolution_clock::now();
  std::vector<double> conditional_dist = inference.prob_cond_bin();
  auto end_time = std::chrono::high_resolution_clock::now();
  if (use_output_file) {
    inference.PrintConditionalDistribution(conditional_dist, output_filename);
  } else {
    inference.PrintConditionalDistribution(conditional_dist, "");
  }
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  long long execution_time_us = duration.count();
  double seconds = execution_time_us / 1000000.0;
  int num_cond_vars = 0;
  int num_interest_vars = 0;
  std::vector<int> maskC = inference.maskC();
  std::vector<int> maskI = inference.maskI();
  for (int val : maskC) {
    if (val == 1) num_cond_vars++;
  }
  for (int val : maskI) {
    if (val == 1) num_interest_vars++;
  }
  // Save data to CSV
  SaveExecutionToCSV(inference.number_of_variables(), 
                    num_cond_vars, 
                    num_interest_vars,
                    execution_time_us,
                    conditional_dist.size());
  // Show execution time summary
  std::cout << "\n=== RESUMEN DE TIEMPOS ===" << std::endl;
  std::cout << "Número total de variables (N): " << inference.number_of_variables() << std::endl;
  std::cout << "Configuraciones totales: 2^" << inference.number_of_variables() 
            << " = " << total_configs << std::endl;
  std::cout << "Variables condicionadas: " << num_cond_vars << std::endl;
  std::cout << "Variables de interés: " << num_interest_vars << std::endl;
  std::cout << "Tamaño distribución condicional: 2^" << num_interest_vars 
            << " = " << conditional_dist.size() << " elementos" << std::endl;
  std::cout << "Tiempo total: " << std::fixed << std::setprecision(6) << seconds << " segundos" << std::endl;
  if (use_output_file) {
    std::cout << "\nLos resultados detallados se han guardado en: " << output_filename << std::endl;
  }
  std::cout << "Las métricas de tiempo se han guardado en: output.csv" << std::endl;
  return 0;
}