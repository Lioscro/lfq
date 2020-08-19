#include "reads.hpp"

const uint8_t Reads::BASES_BYTES = 4;

Reads::Reads(const std::string& filename, Mode mode) {
  this->open(filename, mode);
}

Reads::~Reads() { this->file.close(); }

void Reads::open(const std::string& filename, Mode mode) {
  switch (mode) {
    case Mode::Read:
      this->file.open(filename, std::fstream::in | std::fstream::binary);
      break;
    case Mode::Write:
      this->file.open(filename, std::fstream::out | std::fstream::binary |
                                    std::fstream::trunc);
      break;
    default:
      throw std::string("Unknown file mode: ") + std::to_string(mode);
  }

  if (this->file.bad() || this->file.fail()) {
    throw std::string("Bad stream");
  }
}

size_t Reads::read_n_bases() {
  size_t n_bases = 0;
  char* n_bases_buffer = new char[this->BASES_BYTES]();
  this->file.read(n_bases_buffer, this->BASES_BYTES);
  if (this->file.eof()) {
    return 0;
  }
  for (uint8_t i = 0; i < this->BASES_BYTES; i++) {
    n_bases += n_bases_buffer[i] << (8 * (this->BASES_BYTES - i - 1));
  }
  delete[] n_bases_buffer;
  return n_bases;
}

char* Reads::read_sequence(size_t vector_size) {
  char* buffer = new char[vector_size]();
  this->file.read(buffer, vector_size);
  if (this->file.eof()) {
    delete[] buffer;
    return nullptr;
  }
  return buffer;
}

void Reads::write_sequence_chunk(const Sequence& sequence) {
  size_t size;
  char* chunk =
      reinterpret_cast<char*>(sequence.to_chunk(this->BASES_BYTES, &size));
  this->file.write(chunk, size);
  delete[] chunk;
}

Sequence* Reads::read_sequence_chunk() {
  size_t n_bases = this->read_n_bases();
  size_t vector_size =
      ceil(static_cast<float>(n_bases) / static_cast<float>(3));

  // If n_bases is 0, we reached eof.
  if (n_bases == 0) {
    return nullptr;
  }
  char* buffer = this->read_sequence(vector_size);
  if (buffer == nullptr) {
    delete[] buffer;
    return nullptr;
  }
  Sequence* s =
      new Sequence(reinterpret_cast<uint8_t*>(buffer), vector_size, n_bases);
  delete[] buffer;
  return s;
}
