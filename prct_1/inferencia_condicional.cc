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
#include <iomanip>
#include <cmath>

/**
 * @brief Helper function to print to file or console.
 */
void Inference::PrintToFileOrConsole(std::ostream& os, const std::string& content) const {
  os << content;
}

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
 * @brief Default constructor that initializes the inference object
 * with zero variables.
 */
Inference::Inference() : number_of_variables_(0) {
}

/**
 * @brief Constructor that initializes the inference object with the number of 
 * variables introduced by the user and calling the function that generates
 * a random probability distribution.
 */
Inference::Inference(int num_variables) : number_of_variables_(num_variables) {
  int num_configurations = static_cast<int>(std::pow(2, num_variables));
  probabilities_ = RandomProbabilities(num_configurations);
  InitializeMasks();
}

/**
 * @brief Method that initializes the masks and value vectors for the inference
 * process. All masks are resized according to the number of variables and
 * initialized to zero.
 */
void Inference::InitializeMasks() {
  maskC_.resize(number_of_variables_, 0);
  valC_.resize(number_of_variables_, 0);
  maskI_.resize(number_of_variables_, 0);
  maskM_.resize(number_of_variables_, 0);
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
  std::vector<std::pair<std::string, double>> entries;
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    auto pos = line.find(',');
    std::string mask = line.substr(0, pos);
    std::string prob_str = line.substr(pos + 1);
    double probability = std::stod(prob_str);
    entries.push_back({mask, probability});
  }
  int num_vars = entries[0].first.length();
  inference_to_read.setNumberOfVariables(num_vars);
  int num_configurations = static_cast<int>(std::pow(2, num_vars));
  std::vector<double> probabilities(num_configurations, 0.0);
  for (const auto& entry : entries) {
    int index = BinaryToDecimal(entry.first);
    probabilities[index] = entry.second;
  }
  inference_to_read.setProbabilities(probabilities);
  inference_to_read.InitializeMasks();
  return in;
}


/**
 * @brief Prompts the user to select conditional and interest variables.
 * The user is asked to input the indices and values of the conditional variables,
 * followed by the indices of the variables of interest. The masks and values are
 * stored internally in the inference object.
 */
void Inference::AskVariables() {
  std::cout << "\n SELECCIÓN DE VARIABLES " << std::endl;
  std::cout << "\nVARIABLES CONDICIONADAS (X_C = valores)" << std::endl;
  std::cout << "Ingrese pares 'índice valor' separados por espacios." << std::endl;
  std::cout << "Ejemplo: '2 1 3 0' significa X2=1 y X3=0" << std::endl;
  std::cout << "Pulse enter si no quiere ninguna variable condicional" << std::endl;
  std::cout << ">> ";
  std::string input;
  std::getline(std::cin, input);
  if (!input.empty()) {
    std::istringstream iss(input);
    int index, value;   
    while (iss >> index >> value) {
      maskC_[index - 1] = 1;
      valC_[index - 1] = value;
    }
  }
  std::cout << "\nVARIABLES DE INTERÉS (X_I)" << std::endl;
  std::cout << "Ingrese los índices de las variables separados por espacios." << std::endl;
  std::cout << "Ejemplo: '1 3 4' para X1, X3, X4" << std::endl;
  std::cout << ">> "; 
  std::getline(std::cin, input);
  if (!input.empty()) {
    std::istringstream iss(input);
    int index;   
    while (iss >> index) {
      maskI_[index - 1] = 1;
    }
  } 
  for (int i = 0; i < number_of_variables_; ++i) {
    if (maskI_[i] == 0 && maskC_[i] == 0) {
      maskM_[i] = 1;
    }
  }
}

/**
 * Function that converts a decimal number into its binary representation.
 * This is used to obtain the values of the variables for a given index.
 * @param[in] decimal_number Integer value to be converted to binary.
 * @param[in] size_of_the_binary Number of bits (variables) in the result.
 * @return A vector containing the binary representation of the input number.
 */
std::vector<int> DecimalToBinary(int decimal_number, int size_of_the_binary) {
  std::vector<int> values(size_of_the_binary, 0);
  int temp = decimal_number;
  for (int i = 0; i < size_of_the_binary; ++i) {
    values[size_of_the_binary - 1 - i] = temp % 2;
    temp = temp / 2;
  }
  return values;
}

/**
 * Calculates the conditional probability distribution based on the
 * selected conditioned and interest variables.
 * @return A vector containing the normalized conditional probabilities
 * for all combinations of the interest variables.
 */
std::vector<double> Inference::prob_cond_bin() {
  int num_interest_vars = std::count(maskI_.begin(), maskI_.end(), 1);
  int num_combinations = static_cast<int>(std::pow(2, num_interest_vars));
  std::vector<double> conditional_distribution(num_combinations, 0.0);
  double denominator = 0.0;  
  for (int index_row = 0; index_row < probabilities_.size(); ++index_row) { 
    bool satisfies_conditions = true;
    std::vector<int> values_of_variables_row = DecimalToBinary(index_row, number_of_variables());
    for (int i = 0; i < number_of_variables_; ++i) {
      if (maskC_[i] == 1) {
        if (values_of_variables_row[i] != valC_[i]) {
          satisfies_conditions = false;
          break;
        }
      }
    }
    if (satisfies_conditions) {
      int pattern_index = CalculatePatternIndex(values_of_variables_row);
      conditional_distribution[pattern_index] += probabilities_[index_row];
      denominator += probabilities_[index_row];
    }
  }
  if (denominator > 0) {
    for (int i = 0; i < num_combinations; ++i) {
      conditional_distribution[i] /= denominator;
    }
  }
  return conditional_distribution;
}

/**
 * @brief Translates to a decimal index the pattern (binary) for the interest variable of a row
 * @param values Values of interest variables in a row
 * @return Decimal index
 */
int Inference::CalculatePatternIndex(const std::vector<int>& values) const {
  std::string binary_string = "";
  for (int variable = 0; variable < number_of_variables_; ++variable) {
    if (maskI_[variable] == 1) {
      binary_string += (values[variable] == 1) ? '1' : '0';
    }
  }
  if (binary_string.empty()) {
    return 0;
  }
  return BinaryToDecimal(binary_string);
}


/**
 * @brief Prints the joint distribution generated to file or console.
 * @param filename If empty, prints to console; otherwise prints to file.
 */
void Inference::PrintJointDistribution(const std::string& filename) const {
  std::ostream* output_stream = &std::cout;
  std::ofstream file_stream; 
  if (!filename.empty()) {
    file_stream.open(filename);
    if (!file_stream.is_open()) {
      std::cerr << "Error: No se pudo abrir el archivo " << filename << " para escritura." << std::endl;
      return;
    }
    output_stream = &file_stream;
    std::cout << "Escribiendo distribución conjunta en archivo: " << filename << std::endl;
  }
  std::ostream& os = *output_stream;
  os << "DISTRIBUCIÓN CONJUNTA" << std::endl;
  os << "Número de variables: " << number_of_variables_ << std::endl;
  os << "Número de configuraciones: " << probabilities_.size() << std::endl;
  os << std::string(60, '-') << std::endl;
  if (probabilities_.size() > 1024) {
    os << "Distribución demasiado grande para mostrar completa (" 
       << probabilities_.size() << " configuraciones)." << std::endl;
    os << "Mostrando solo las primeras y últimas 5 configuraciones:" << std::endl << std::endl;
    for (int i = 1; i <= number_of_variables_; ++i) {
      os << "X" << i << " ";
    }
    os << "| Probabilidad" << std::endl;
    os << std::string(60, '-') << std::endl;   
    for (size_t config = 0; config < 5 && config < probabilities_.size(); ++config) {
      std::vector<int> values = DecimalToBinary(config, number_of_variables_);     
      for (int i = 0; i < number_of_variables_; ++i) {
        os << values[i] << "  ";
      }     
      os << "| " << std::fixed << std::setprecision(6) << probabilities_[config] << std::endl;
    }   
    os << "... (" << (probabilities_.size() - 10) << " configuraciones omitidas) ..." << std::endl;   
    for (size_t config = probabilities_.size() - 5; config < probabilities_.size(); ++config) {
      std::vector<int> values = DecimalToBinary(config, number_of_variables_);     
      for (int i = 0; i < number_of_variables_; ++i) {
        os << values[i] << "  ";
      }     
      os << "| " << std::fixed << std::setprecision(6) << probabilities_[config] << std::endl;
    }
  } else {
    for (int i = 1; i <= number_of_variables_; ++i) {
      os << "X" << i << " ";
    }
    os << "| Probabilidad" << std::endl;
    os << std::string(60, '-') << std::endl;    
    double total_sum = 0.0;
    for (size_t config = 0; config < probabilities_.size(); ++config) {
      std::vector<int> values = DecimalToBinary(config, number_of_variables_);     
      for (int i = 0; i < number_of_variables_; ++i) {
        os << values[i] << "  ";
      }     
      os << "| " << std::fixed << std::setprecision(6) << probabilities_[config] << std::endl;
      total_sum += probabilities_[config];
    }   
    os << std::string(60, '-') << std::endl;
    os << "Suma total de probabilidades: " << std::fixed << std::setprecision(6) 
       << total_sum << std::endl;
  }
  if (!filename.empty()) {
    file_stream.close();
  }
}

/**
 * Prints the conditional probability distribution either to the console
 * or to a file. 
 * @param[in] conditional_distribution Vector containing the normalized
 * conditional probabilities.
 * @param[in] filename Name of the output file. If empty, the distribution
 * is printed to the console; otherwise, it is appended to the file.
 */
void Inference::PrintConditionalDistribution(const std::vector<double>& conditional_distribution,
                                           const std::string& filename) const {
  std::ostream* output_stream = &std::cout;
  std::ofstream file_stream;  
  if (!filename.empty()) {
    file_stream.open(filename, std::ios::app);
    if (!file_stream.is_open()) {
      std::cerr << "Error: No se pudo abrir el archivo " << filename << " para escritura." << std::endl;
      return;
    }
    output_stream = &file_stream;
    if (file_stream.tellp() == 0) {
      std::cout << "Escribiendo distribución condicional en archivo: " << filename << std::endl;
    } else {
      std::cout << "Añadiendo distribución condicional al archivo: " << filename << std::endl;
    }
  } 
  std::ostream& os = *output_stream; 
  std::vector<std::pair<int, int>> conditioned_vars;
  for (int i = 0; i < number_of_variables_; ++i) {
    if (maskC_[i] == 1) {
      conditioned_vars.push_back({i + 1, valC_[i]});
    }
  }  
  std::vector<int> interest_vars;
  for (int i = 0; i < number_of_variables_; ++i) {
    if (maskI_[i] == 1) {
      interest_vars.push_back(i + 1);
    }
  }  
  os << "\n DISTRIBUCIÓN CONDICIONAL " << std::endl;  
  os << "Variables condicionadas (X_C): ";
  if (conditioned_vars.empty()) {
    os << "ninguna";
  } else {
    for (size_t i = 0; i < conditioned_vars.size(); ++i) {
      os << "X" << conditioned_vars[i].first << "=" << conditioned_vars[i].second;
      if (i < conditioned_vars.size() - 1) {
        os << ", ";
      }
    }
  }
  os << std::endl; 
  os << "Variables de interés (X_I): ";
  if (interest_vars.empty()) {
    os << "ninguna";
  } else {
    for (size_t i = 0; i < interest_vars.size(); ++i) {
      os << "X" << interest_vars[i];
      if (i < interest_vars.size() - 1) {
        os << ", ";
      }
    }
  }
  os << std::endl;
  os << "\nProbabilidades condicionales P(X_I | X_C):" << std::endl;
  os << std::string(60, '-') << std::endl;
  for (int var : interest_vars) {
    os << "X" << var << "\t";
  }
  os << "| Probabilidad" << std::endl;
  os << std::string(60, '-') << std::endl; 
  for (size_t comb = 0; comb < conditional_distribution.size(); ++comb) {
    std::vector<int> comb_values(interest_vars.size(), 0);
    int temp = comb;    
    for (int j = interest_vars.size() - 1; j >= 0; --j) {
      comb_values[j] = temp % 2;
      temp = temp / 2;
    }    
    for (int val : comb_values) {
      os << val << "\t";
    }   
    os << "| " << std::fixed << std::setprecision(6) << conditional_distribution[comb] << std::endl;
  } 
  os << std::string(60, '-') << std::endl;
  double sum = 0.0;
  for (double prob : conditional_distribution) {
    sum += prob;
  }
  os << "Suma total: " << std::fixed << std::setprecision(6) << sum << std::endl;
  if (!filename.empty()) {
    file_stream.close();
  }
}