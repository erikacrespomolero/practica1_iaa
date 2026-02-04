/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correos: alu0101655659@ull.edu.es y alu0101639185@ull.edu.es
 * Fecha: 04/02/2026
 * Implementation of Inference class.
 * Allows calculating conditional distributions from a discrete joint
 * distribution of binary variables, using variable marginalization.
 */

#include <random>
#include "inferencia_condicional.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

/**
 * @brief Function that randomly generates positive values and
 * normalizes them so that the sum of probabilities is 1.
 * @param[in] number_of_variables Number of values to generate in the distribution.
 * @param[in] seed Seed used to initialize the random number generator.
 * @return A vector containing normalized random probabilities whose sum is 1.
 */
std::vector<double> RandomProbabilities(int number_of_variables, unsigned seed) {
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

/**
 * @brief Function that converts a binary string into its decimal representation.
 * This is how the index of the variable is obtained.
 * @param[in] s String containing a binary number.
 * @return Integer value corresponding to the binary input string in decimal.
 */
int BinaryToDecimal(const std::string &s) {
  int value = 0;
  for (char c : s) {
    value = (value << 1) | static_cast<int>(c - '0');
  }
  return value;
}

/**
 * @brief Constructor that initializes the inference object by asking the user
 * for the number of variables and calling the function that generates
 * a random probability distribution.
 */
Inference::Inference() {
  int number_of_variables{0};
  std::cout << "Diga cuántas variables quiere: ";
  std::cin >> number_of_variables;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  number_of_variables_ = number_of_variables;
  probabilities_ = RandomProbabilities(pow(2, number_of_variables));
}

/**
 * @brief Input stream operator overloading (>>).
 * Reads a probability distribution from the input stream, where each line
 * contains a binary mask and its associated probability separated by ','.
 * The binary mask is converted to decimal and used as an index to store
 * the probability in the probabilities vector.
 * @param[in] in Reference to std::istream.
 * @param[in] inference_to_read Reference to an Inference object where the
 * probabilities read from the stream are stored.
 * @return in The input stream after reading the probability distribution.
 */
std::istream& operator>>(std::istream& in, Inference& inference_to_read) {
  std::string line;
  if (!std::getline(in, line)) return in;
  auto pos = line.find(',');
  std::string mask = line.substr(0, pos);
  std::string probability_str = line.substr(pos + 1);
  double probability = std::stod(probability_str);
  std::vector<double> probabilities;
  int number_of_variables = mask.length();
  inference_to_read.setNumberOfVariables(number_of_variables);
  int number_of_lines = pow(2, number_of_variables) - 1;
  probabilities.resize(number_of_lines + 1);
  probabilities[BinaryToDecimal(mask)] = probability;
  while (number_of_lines > 0) {
    if (!std::getline(in, line)) break;
    pos = line.find(',');
    mask = line.substr(0, pos);
    probability_str = line.substr(pos + 1);
    probability = std::stod(probability_str);
    probabilities[BinaryToDecimal(mask)] = probability;
  }
  inference_to_read.setProbabilities(probabilities);
  return in;
}

/**
 * @brief Prompts the user to select conditional and interest variables.
 * The user is asked to input the indices and values of the conditional variables,
 * followed by the indices of the variables of interest. The masks and values are
 * stored internally in the inference object.
 */
void Inference::AskVariables() {
  std::string input;
  // Condicional variables
  std::cout << "Introduzca las variables que quiere que sean condicionales (índices) cada una seguida de su valor y separadas por espacios, por ejemplo 2 0 3 1 6 0: ";
  std::getline(std::cin, input);
  std::istringstream iss_condicionals(input);
  int index, value;
  maskC_.resize(number_of_variables());
  valC_.resize(number_of_variables());
  std::fill(maskC_.begin(), maskC_.end(), 0);
  std::fill(valC_.begin(), valC_.end(), 0);
  while (iss_condicionals >> index >> value) {
    maskC_[index - 1] = 1;
    valC_[index - 1] = value;
  }
  // Interest variables
  std::cout << "Introduzca las variables que quiere que sean de interés (índices), por ejemplo 1 4 5: ";
  std::getline(std::cin, input);
  std::istringstream iss_interest(input);
  maskI_.resize(number_of_variables());
  std::fill(maskI_.begin(), maskI_.end(), 0);
  while (iss_interest >> index) {
    maskI_[index - 1] = 1;
  }
}

