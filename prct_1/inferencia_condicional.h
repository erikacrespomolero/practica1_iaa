/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correos: alu0101655659@ull.edu.es y alu0101639185@ull.edu.es
 * Fecha: 04/02/2026
 * Declaration of Inference class.
 * Allows calculating conditional distributions from a discrete joint
 * distribution of binary variables, using variable marginalization.
 */

#ifndef INFERENCE
#define INFERENCE

#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <random>

/** 
 * Class Inference.
 *       Represents and manages a probability distribution associated
 *       with a set of variables, providing functionality for initialization,
 *       access, and input stream loading.
 */
class Inference {
 public:
  Inference();
  Inference(std::string file) {}
  int number_of_variables() { return number_of_variables_; }
  int_fast32_t number_of_variables() const { return number_of_variables_; }
  std::vector<double> probabilities() { return probabilities_; }
  std::vector<double> probabilities() const { return probabilities_; }
  void setProbabilities(std::vector<double> probabilities) { probabilities_ = probabilities; }
  void askVariables();
 private:
  int number_of_variables_;
  std::vector<double> probabilities_; 
  std::vector<int> maskC_;
  std::vector<int> valC_;
  std::vector<int> maskI_;
};

std::vector<double> RandomProbabilities(size_t number_of_variables, unsigned seed = std::random_device{}());
int BinaryToDecimal(const std::string &s);
std::istream& operator>>(std::istream& in, Inference& inference_to_read);

#endif