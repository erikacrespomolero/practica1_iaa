/** Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial Avanzada  
 * Curso: 3o
 * Autoras: Candela García Cruz y Érika Crespo Molero
 * Correo: alu0101639185@ull.edu.es
 */

#ifndef INFERENCE
#define INFERENCE

#include <utility>
#include <vector>
#include <iostream>
#include <string>

class Inference {
 public:
  Inference();
  Inference(std::string file) {}
  int number_of_variables() { return number_of_variables_; }
  int_fast32_t number_of_variables() const { return number_of_variables_; }
  std::vector<double> probabilities() { return probabilities_; }
  std::vector<double> probabilities() const { return probabilities_; }
  void setProbabilities(std::vector<double> probabilities) { probabilities_ = probabilities; }

 private:
  int number_of_variables_;
  std::vector<double> probabilities_; 
};

std::vector<double> RandomProbabilities(size_t number_of_variables, unsigned seed = std::random_device{}());
int BinaryToDecimal(const std::string &s);
std::istream& operator>>(std::istream& in, Inference& inference_to_read);

#endif