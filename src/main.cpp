#include <iostream>

#include "sequence.hpp"

int main(int argc, char* argv[]) {
  // char test[] = {'A', 'C', 'T', 'G', 'C', 'C', 'T', 'A'};
  // Sequence s = Sequence::encode(test, 8);
  // for (size_t i = 0; i < s.sequence.size(); i++) {
  //   std::cout << unsigned(s.sequence.at(i)) << std::endl;
  // }

  uint8_t test[] = {47, 70, 241};
  Sequence s(test, (size_t)3, (size_t)8);
  char* decoded = s.decode();
  for (size_t i = 0; i < s.n_bases; i++) {
    std::cout << decoded[i] << std::endl;
  }
  return 0;
}
