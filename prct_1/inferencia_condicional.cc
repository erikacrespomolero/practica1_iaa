/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correo: alu0101639185@ull.edu.es
 */

#include <random>
#include "inferencia_condicional.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>


std::vector<double> RandomProbabilities(int number_of_variables, unsigned seed = std::random_device{}()) {
    std::mt19937 gen(seed);
    std::exponential_distribution<double> expd(1.0);
    std::vector<double> vector_of_randoms(number_of_variables);
    double sum = 0.0;
    for (int i = 0; i < number_of_variables; ++i) {
        vector_of_randoms[i] = expd(gen);
        sum += vector_of_randoms[i];
    }
    for (int i = 0; i < number_of_variables; ++i) vector_of_randoms[i] /= sum;
    return vector_of_randoms;
}

  // Convierte una cadena binaria del estilo "1011" a entero decimal.
  // Lanza std::invalid_argument si la cadena contiene caracteres distintos de '0' o '1'.
  int BinaryToDecimal(const std::string &s) {
    int value = 0;
    for (char c : s) {
      value = (value << 1) | static_cast<int>(c - '0');
    }
    return value;
  }


Inference::Inference() {
  int number_of_variables{0};
  std::cout << "Diga cuántas variables quiere: ";
  std::cin >> number_of_variables;
  probabilities_ = RandomProbabilities(number_of_variables);
}


std::istream& operator>>(std::istream& in, Inference& inference_to_read) {
  std::string line;
  if (!std::getline(in, line)) return in;
  auto pos = line.find(',');
  std::string mask = line.substr(0, pos);
  std::string probability_str = line.substr(pos + 1);
  double probability = std::stod(probability_str);
  std::vector<double> probabilities;
  int number_of_variables = mask.length();
  int number_of_lines = pow(2, number_of_variables) - 1;
  probabilities.resize(number_of_lines + 1);
  probabilities[BinaryToDecimal(mask)] = probability;
  while (number_of_lines > 0) {
    if (!std::getline(in, line)) return in;
    pos = line.find(',');
    mask = line.substr(0, pos);
    probability_str = line.substr(pos + 1);
    probability = std::stod(probability_str);
    probabilities[BinaryToDecimal(mask)] = probability;
  }
  inference_to_read.setProbabilities(probabilities);
  return in;
}

