#ifndef SRC_READS_HPP_
#define SRC_READS_HPP_
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "sequence.hpp"

enum Mode { Read, Write };

class Reads {
 private:
  static const uint8_t BASES_BYTES;

  std::fstream file;
  void open(const std::string& filename, Mode mode);
  size_t read_n_bases();
  char* read_sequence(size_t vector_size);

 public:
  Reads(const std::string& filename, Mode mode);
  ~Reads();

  void write_sequence_chunk(const Sequence& sequence);
  Sequence* read_sequence_chunk();
};

#endif  // SRC_READS_HPP_
