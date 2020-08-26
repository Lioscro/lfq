#include "sequence.hpp"

const size_t Sequence::BLOCK_SIZE = 5;
const size_t Sequence::TYPE_SIZE = sizeof(uint8_t) * 8;
const uint8_t Sequence::MASK = pow(2, Sequence::BLOCK_SIZE) - 1;

const uint8_t Sequence::PAIR_AA = 0x00;
const uint8_t Sequence::PAIR_AT = 0x01;
const uint8_t Sequence::PAIR_AC = 0x02;
const uint8_t Sequence::PAIR_AG = 0x03;
const uint8_t Sequence::PAIR_TA = 0x1e;
const uint8_t Sequence::PAIR_TT = 0x1f;
const uint8_t Sequence::PAIR_TC = 0x1c;
const uint8_t Sequence::PAIR_TG = 0x1d;
const uint8_t Sequence::PAIR_CA = 0x04;
const uint8_t Sequence::PAIR_CT = 0x05;
const uint8_t Sequence::PAIR_CC = 0x06;
const uint8_t Sequence::PAIR_CG = 0x07;
const uint8_t Sequence::PAIR_GA = 0x1a;
const uint8_t Sequence::PAIR_GT = 0x1b;
const uint8_t Sequence::PAIR_GC = 0x18;
const uint8_t Sequence::PAIR_GG = 0x19;
const uint8_t Sequence::PAIR_NN = 0x08;
const uint8_t Sequence::PAIR_NA = 0x09;
const uint8_t Sequence::PAIR_NT = 0x16;
const uint8_t Sequence::PAIR_NC = 0x0a;
const uint8_t Sequence::PAIR_NG = 0x15;
const uint8_t Sequence::PAIR_AN = 0x0b;
const uint8_t Sequence::PAIR_TN = 0x14;
const uint8_t Sequence::PAIR_CN = 0x0c;
const uint8_t Sequence::PAIR_GN = 0x13;
const uint8_t Sequence::PAIR_NB = 0x0d;
const uint8_t Sequence::PAIR_AB = 0x0e;
const uint8_t Sequence::PAIR_TB = 0x11;
const uint8_t Sequence::PAIR_CB = 0x0f;
const uint8_t Sequence::PAIR_GB = 0x10;

Sequence::Sequence() : sequence() {}

Sequence::Sequence(const std::vector<uint8_t>& sequence, size_t n_bases)
    : sequence(sequence.begin(), sequence.end()), n_bases(n_bases) {}

Sequence::Sequence(const uint8_t* sequence, size_t size, size_t n_bases)
    : sequence(sequence, sequence + size), n_bases(n_bases) {}

const std::vector<uint8_t>& Sequence::get_sequence() const {
  return this->sequence;
}

size_t Sequence::get_n_bases() const { return this->n_bases; }

size_t Sequence::get_n_blocks() const {
  return this->get_n_blocks(this->n_bases);
}

size_t Sequence::get_n_blocks(size_t n_bases) {
  return (n_bases / 2) + (n_bases % 2);
}

size_t Sequence::get_vector_size() const {
  return this->get_vector_size(this->get_n_blocks());
}

size_t Sequence::get_vector_size(size_t n_blocks) {
  return ceil(static_cast<float>(n_blocks * Sequence::BLOCK_SIZE) /
              Sequence::TYPE_SIZE);
}

uint8_t Sequence::pair_to_encoding(char c1, char c2) {
  switch (c1) {
    case 'A':
    case 'a':
      switch (c2) {
        case 'A':
        case 'a':
          return Sequence::PAIR_AA;
        case 'C':
        case 'c':
          return Sequence::PAIR_AC;
        case 'G':
        case 'g':
          return Sequence::PAIR_AG;
        case 'T':
        case 't':
          return Sequence::PAIR_AT;
        case 'N':
        case 'n':
          return Sequence::PAIR_AN;
        case 0:
          return Sequence::PAIR_AB;
        default:
          throw std::runtime_error("Unrecognized base " + std::string(1, c2));
      }
    case 'C':
    case 'c':
      switch (c2) {
        case 'A':
        case 'a':
          return Sequence::PAIR_CA;
        case 'C':
        case 'c':
          return Sequence::PAIR_CC;
        case 'G':
        case 'g':
          return Sequence::PAIR_CG;
        case 'T':
        case 't':
          return Sequence::PAIR_CT;
        case 'N':
        case 'n':
          return Sequence::PAIR_CN;
        case 0:
          return Sequence::PAIR_CB;
        default:
          throw std::runtime_error("Unrecognized base " + std::string(1, c2));
      }
    case 'G':
    case 'g':
      switch (c2) {
        case 'A':
        case 'a':
          return Sequence::PAIR_GA;
        case 'C':
        case 'c':
          return Sequence::PAIR_GC;
        case 'G':
        case 'g':
          return Sequence::PAIR_GG;
        case 'T':
        case 't':
          return Sequence::PAIR_GT;
        case 'N':
        case 'n':
          return Sequence::PAIR_GN;
        case 0:
          return Sequence::PAIR_GB;
        default:
          throw std::runtime_error("Unrecognized base " + std::string(1, c2));
      }
    case 'T':
    case 't':
      switch (c2) {
        case 'A':
        case 'a':
          return Sequence::PAIR_TA;
        case 'C':
        case 'c':
          return Sequence::PAIR_TC;
        case 'G':
        case 'g':
          return Sequence::PAIR_TG;
        case 'T':
        case 't':
          return Sequence::PAIR_TT;
        case 'N':
        case 'n':
          return Sequence::PAIR_TN;
        case 0:
          return Sequence::PAIR_TB;
        default:
          throw std::runtime_error("Unrecognized base " + std::string(1, c2));
      }
    case 'N':
    case 'n':
      switch (c2) {
        case 'A':
        case 'a':
          return Sequence::PAIR_NA;
        case 'C':
        case 'c':
          return Sequence::PAIR_NC;
        case 'G':
        case 'g':
          return Sequence::PAIR_NG;
        case 'T':
        case 't':
          return Sequence::PAIR_NT;
        case 'N':
        case 'n':
          return Sequence::PAIR_NN;
        case 0:
          return Sequence::PAIR_NB;
        default:
          throw std::runtime_error("Unrecognized base" + std::string(1, c2));
      }
    default:
      throw std::runtime_error("Unrecognized base" + std::string(1, c1));
  }
}

char* Sequence::encoding_to_pair(const uint8_t e) {
  char* pair = new char[2]();
  switch (e) {
    case Sequence::PAIR_AA:
      pair[0] = 'A';
      pair[1] = 'A';
      break;
    case Sequence::PAIR_AT:
      pair[0] = 'A';
      pair[1] = 'T';
      break;
    case Sequence::PAIR_AC:
      pair[0] = 'A';
      pair[1] = 'C';
      break;
    case Sequence::PAIR_AG:
      pair[0] = 'A';
      pair[1] = 'G';
      break;
    case Sequence::PAIR_TA:
      pair[0] = 'T';
      pair[1] = 'A';
      break;
    case Sequence::PAIR_TT:
      pair[0] = 'T';
      pair[1] = 'T';
      break;
    case Sequence::PAIR_TC:
      pair[0] = 'T';
      pair[1] = 'C';
      break;
    case Sequence::PAIR_TG:
      pair[0] = 'T';
      pair[1] = 'G';
      break;
    case Sequence::PAIR_CA:
      pair[0] = 'C';
      pair[1] = 'A';
      break;
    case Sequence::PAIR_CT:
      pair[0] = 'C';
      pair[1] = 'T';
      break;
    case Sequence::PAIR_CC:
      pair[0] = 'C';
      pair[1] = 'C';
      break;
    case Sequence::PAIR_CG:
      pair[0] = 'C';
      pair[1] = 'G';
      break;
    case Sequence::PAIR_GA:
      pair[0] = 'G';
      pair[1] = 'A';
      break;
    case Sequence::PAIR_GT:
      pair[0] = 'G';
      pair[1] = 'T';
      break;
    case Sequence::PAIR_GC:
      pair[0] = 'G';
      pair[1] = 'C';
      break;
    case Sequence::PAIR_GG:
      pair[0] = 'G';
      pair[1] = 'G';
      break;
    case Sequence::PAIR_NN:
      pair[0] = 'N';
      pair[1] = 'N';
      break;
    case Sequence::PAIR_NA:
      pair[0] = 'N';
      pair[1] = 'A';
      break;
    case Sequence::PAIR_NT:
      pair[0] = 'N';
      pair[1] = 'T';
      break;
    case Sequence::PAIR_NC:
      pair[0] = 'N';
      pair[1] = 'C';
      break;
    case Sequence::PAIR_NG:
      pair[0] = 'N';
      pair[1] = 'G';
      break;
    case Sequence::PAIR_AN:
      pair[0] = 'A';
      pair[1] = 'N';
      break;
    case Sequence::PAIR_TN:
      pair[0] = 'T';
      pair[1] = 'N';
      break;
    case Sequence::PAIR_CN:
      pair[0] = 'C';
      pair[1] = 'N';
      break;
    case Sequence::PAIR_GN:
      pair[0] = 'G';
      pair[1] = 'N';
      break;
    case Sequence::PAIR_NB:
      pair[0] = 'N';
      pair[1] = 0;
      break;
    case Sequence::PAIR_AB:
      pair[0] = 'A';
      pair[1] = 0;
      break;
    case Sequence::PAIR_TB:
      pair[0] = 'T';
      pair[1] = 0;
      break;
    case Sequence::PAIR_CB:
      pair[0] = 'C';
      pair[1] = 0;
      break;
    case Sequence::PAIR_GB:
      pair[0] = 'G';
      pair[1] = 0;
      break;
    default:
      throw std::runtime_error("Unrecognized encoding " + std::to_string(e));
  }
  return pair;
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
  size_t n_blocks = s->get_n_blocks(n_bases);
  size_t vector_size = s->get_vector_size(n_blocks);
  char c1;
  char c2;

  s->n_bases = n_bases;
  s->sequence.reserve(vector_size);

  for (size_t i = 0; i < n_blocks; i++) {
    bit_i = i * s->BLOCK_SIZE;
    vector_i = bit_i / s->TYPE_SIZE;

    // Convert pair to encoding.
    c1 = str[i * 2];
    c2 = (i * 2 + 1) < n_bases ? str[i * 2 + 1] : 0;
    base = s->pair_to_encoding(c1, c2);

    overflow = (bit_i + s->BLOCK_SIZE) - ((vector_i + 1) * s->TYPE_SIZE);
    if (overflow >= 0) {
      // The encoding needs to span two vector elements.
      block += base >> overflow;
      s->sequence.push_back(block);
      block = base << (s->TYPE_SIZE - overflow);
    } else {
      // The encoding is within one vector element.
      block += base << -overflow;
    }
  }
  if ((bit_i + s->BLOCK_SIZE) % (s->TYPE_SIZE) != 0) {
    s->sequence.push_back(block);
  }
  return s;
}

std::string Sequence::decode() const {
  uint8_t encoded = 0;
  int8_t overflow = 0;
  size_t bit_i = 0;
  size_t vector_i = 0;
  size_t n_blocks = this->get_n_blocks();
  char* decoded = new char[this->n_bases]();
  char* pair = nullptr;

  for (size_t i = 0; i < n_blocks; i++) {
    bit_i = i * this->BLOCK_SIZE;
    vector_i = bit_i / Sequence::TYPE_SIZE;

    overflow = (bit_i + this->BLOCK_SIZE) - ((vector_i + 1) * this->TYPE_SIZE);
    if (overflow > 0) {
      // The encoding spans two vector elements.
      encoded =
          ((this->sequence[vector_i] & (this->MASK >> overflow)) << overflow) +
          (this->sequence[vector_i + 1] >> (this->TYPE_SIZE - overflow));
    } else {
      // The base is in one vector element.
      encoded = (this->sequence[vector_i] >> -overflow) & this->MASK;
    }
    pair = this->encoding_to_pair(encoded);
    decoded[i * 2] = pair[0];
    decoded[i * 2 + 1] = pair[1];
    delete[] pair;
  }

  std::string result = std::string(decoded, this->n_bases);
  delete[] decoded;
  return result;
}

size_t Sequence::get_chunk_size(uint8_t n_bases_bytes) const {
  return n_bases_bytes + this->sequence.size();
}

uint8_t* Sequence::to_chunk(uint8_t n_bases_bytes) const {
  // Make sure n_bases_bytes can hold n_bases.
  if (this->n_bases > pow(2, n_bases_bytes * this->TYPE_SIZE) - 1) {
    throw std::runtime_error(
        std::string("Can not encode ") + std::to_string(this->n_bases) +
        std::string(" in ") + std::to_string(n_bases_bytes) +
        std::string(" bytes"));
  }

  size_t size = this->get_chunk_size(n_bases_bytes);
  uint8_t* chunk = new uint8_t[size]();
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
