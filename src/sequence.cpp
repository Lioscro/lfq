#include "sequence.hpp"

const size_t Sequence::BLOCK_SIZE = 3;
const size_t Sequence::TYPE_SIZE = sizeof(uint8_t) * 8;
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

const std::vector<uint8_t> Sequence::get_sequence() const {
  return this->sequence;
}

size_t Sequence::get_n_bases() const { return this->n_bases; }

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

Sequence* Sequence::encode(const std::string& str) {
  return Sequence::encode(str.c_str(), str.size());
}

Sequence* Sequence::encode(const char* str, size_t n_bases) {
  uint8_t base = 0;
  uint8_t block = 0;
  int8_t overflow = 0;
  size_t bit_i = 0;
  size_t vector_i = 0;
  Sequence* s = new Sequence();
  s->n_bases = n_bases;

  for (size_t i = 0; i < n_bases; i++) {
    bit_i = i * s->BLOCK_SIZE;
    vector_i = bit_i / s->TYPE_SIZE;

    // Convert base character to 3-bit encoding.
    base = s->char_to_encoding(str[i]);

    overflow = (bit_i + s->BLOCK_SIZE) - ((vector_i + 1) * s->TYPE_SIZE);
    if (overflow > 0) {
      // The base needs to span two vector elements.
      block += base >> overflow;
      s->sequence.push_back(block);
      block = base << (Sequence::TYPE_SIZE - overflow);
    } else {
      // The base is within one vector element.
      block += base << -overflow;
    }
  }
  s->sequence.push_back(block);

  return s;
}

std::string Sequence::decode() const {
  char base = 0;
  uint8_t encoded = 0;
  int8_t overflow = 0;
  size_t bit_i = 0;
  size_t vector_i = 0;
  std::string decoded = "";

  for (size_t i = 0; i < this->n_bases; i++) {
    bit_i = i * this->BLOCK_SIZE;
    vector_i = bit_i / Sequence::TYPE_SIZE;

    overflow = (bit_i + this->BLOCK_SIZE) - ((vector_i + 1) * this->TYPE_SIZE);
    if (overflow > 0) {
      // The base spans two vector elements.
      encoded =
          ((this->sequence[vector_i] & (this->MASK >> overflow)) << overflow) +
          (this->sequence[vector_i + 1] >> (Sequence::TYPE_SIZE - overflow));
    } else {
      // The base is in one vector element.
      encoded = (this->sequence[vector_i] >> -overflow) & this->MASK;
    }
    decoded += this->encoding_to_char(encoded);
  }

  return decoded;
}

uint8_t* Sequence::to_chunk(uint8_t n_bases_bytes, size_t* size) const {
  // Make sure n_bases_bytes can hold n_bases.
  if (this->n_bases > pow(2, n_bases_bytes * this->TYPE_SIZE) - 1) {
    throw std::runtime_error(
        std::string("Can not encode ") + std::to_string(this->n_bases) +
        std::string(" in ") + std::to_string(n_bases_bytes) +
        std::string(" bytes"));
  }

  *size = n_bases_bytes + this->sequence.size();
  uint8_t* chunk = new uint8_t[*size]();
  // Encode number of bases in n_bases_bytes bytes.
  for (uint8_t i = 0; i < n_bases_bytes; i++) {
    chunk[i] =
        (this->n_bases >> (this->TYPE_SIZE * (n_bases_bytes - i - 1))) & 0xff;
  }
  // Fill the rest
  for (size_t i = 0; i < this->sequence.size(); i++) {
    chunk[i + n_bases_bytes] = this->sequence[i];
  }
  return chunk;
}
