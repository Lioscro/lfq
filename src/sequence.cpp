#include "sequence.hpp"

const size_t Sequence::BLOCK_SIZE = 3;
const uint8_t Sequence::MASK = pow(2, Sequence::BLOCK_SIZE) - 1;
const uint8_t Sequence::BASE_N = 0x00;
const uint8_t Sequence::BASE_A = 0x01;
const uint8_t Sequence::BASE_T = 0x06;
const uint8_t Sequence::BASE_C = 0x03;
const uint8_t Sequence::BASE_G = 0x04;

Sequence::Sequence() : sequence() {}

Sequence::Sequence(const std::vector<uint8_t>& sequence, size_t n_bases)
    : sequence(sequence.begin(), sequence.end()), n_bases(n_bases) {}

Sequence::Sequence(const uint8_t* sequence, size_t size, size_t n_bases)
    : sequence(sequence, sequence + size), n_bases(n_bases) {}

uint8_t Sequence::char_to_encoding(const char c) {
  switch (c) {
    case 'A':
    case 'a':
      return Sequence::BASE_A;
    case 'C':
    case 'c':
      return Sequence::BASE_C;
    case 'G':
    case 'g':
      return Sequence::BASE_G;
    case 'T':
    case 't':
      return Sequence::BASE_T;
    default:
      return Sequence::BASE_N;
  }
}

char Sequence::encoding_to_char(const uint8_t e) {
  switch (e) {
    case 0x01:
      return 'A';
    case 0x06:
      return 'T';
    case 0x03:
      return 'C';
    case 0x04:
      return 'G';
    default:
      return 'N';
  }
}

Sequence& Sequence::encode(const std::string& str) {
  return Sequence::encode(str.c_str(), str.size());
}

Sequence& Sequence::encode(const char* str, size_t n_bases) {
  uint8_t base = 0;
  uint8_t block = 0;
  int8_t overflow = 0;
  size_t bit_i = 0;
  size_t vector_i = 0;
  static Sequence s;
  s.n_bases = n_bases;

  for (size_t i = 0; i < n_bases; i++) {
    bit_i = i * s.BLOCK_SIZE;
    vector_i = bit_i / 8;

    // Convert base character to 3-bit encoding.
    base = s.char_to_encoding(str[i]);

    overflow = (bit_i + s.BLOCK_SIZE) - ((vector_i + 1) * 8);
    if (overflow > 0) {
      // The base needs to span two vector elements.
      block += base >> overflow;
      s.sequence.push_back(block);
      block = base << (8 - overflow);
    } else {
      // The base is within one vector element.
      block += base << -overflow;
    }
  }
  s.sequence.push_back(block);

  return s;
}

char* Sequence::decode() {
  char base = 0;
  uint8_t encoded = 0;
  int8_t overflow = 0;
  size_t bit_i = 0;
  size_t vector_i = 0;
  char* decoded = new char[this->n_bases]();

  for (size_t i = 0; i < this->n_bases; i++) {
    bit_i = i * this->BLOCK_SIZE;
    vector_i = bit_i / 8;

    overflow = (bit_i + this->BLOCK_SIZE) - ((vector_i + 1) * 8);
    if (overflow > 0) {
      // The base spans two vector elements.
      encoded =
          ((this->sequence[vector_i] & (this->MASK >> overflow)) << overflow) +
          (this->sequence[vector_i + 1] >> (8 - overflow));
    } else {
      // The base is in one vector element.
      encoded = (this->sequence[vector_i] >> -overflow) & this->MASK;
    }
    decoded[i] = this->encoding_to_char(encoded);
  }

  return decoded;
}
