#ifndef SRC_SEQUENCE_HPP_
#define SRC_SEQUENCE_HPP_
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Sequence {
 private:
  /* Static constants related to base encodings. */
  static const uint8_t MASK;
  static const uint8_t PAIR_NN;
  static const uint8_t PAIR_NA;
  static const uint8_t PAIR_NT;
  static const uint8_t PAIR_NC;
  static const uint8_t PAIR_NG;
  static const uint8_t PAIR_AN;
  static const uint8_t PAIR_AA;
  static const uint8_t PAIR_AT;
  static const uint8_t PAIR_AC;
  static const uint8_t PAIR_AG;
  static const uint8_t PAIR_TN;
  static const uint8_t PAIR_TA;
  static const uint8_t PAIR_TT;
  static const uint8_t PAIR_TC;
  static const uint8_t PAIR_TG;
  static const uint8_t PAIR_CN;
  static const uint8_t PAIR_CA;
  static const uint8_t PAIR_CT;
  static const uint8_t PAIR_CC;
  static const uint8_t PAIR_CG;
  static const uint8_t PAIR_GN;
  static const uint8_t PAIR_GA;
  static const uint8_t PAIR_GT;
  static const uint8_t PAIR_GC;
  static const uint8_t PAIR_GG;
  static const uint8_t PAIR_NB;
  static const uint8_t PAIR_AB;
  static const uint8_t PAIR_TB;
  static const uint8_t PAIR_CB;
  static const uint8_t PAIR_GB;

  /* The actual encoded sequence & number of (encoded) bases */
  std::vector<uint8_t> sequence;
  size_t n_bases;

  /* Static methods to convert a char to its encoding and vice-versa. */
  static uint8_t pair_to_encoding(const char c1, const char c2);
  static char* encoding_to_pair(const uint8_t e);

 public:
  /* Other static constants, but these can be accessed from other classes. */
  static const size_t BLOCK_SIZE;
  static const size_t TYPE_SIZE;

  /* Constructors. */
  Sequence();
  Sequence(const std::vector<uint8_t>& sequence, size_t n_bases);
  Sequence(const uint8_t* sequence, size_t size, size_t n_bases);

  /* Accessors */
  const std::vector<uint8_t>& get_sequence() const;
  size_t get_n_bases() const;
  static size_t get_type_size();

  /* Utility */
  size_t get_n_blocks() const;
  static size_t get_n_blocks(size_t n_bases);
  size_t get_vector_size() const;
  static size_t get_vector_size(size_t n_blocks);
  size_t get_chunk_size(uint8_t n_bases_bytes) const;

  /* Static "constructor" methods that creates a Sequence object with the
   * provided string/char array encoded.
   */
  static Sequence* encode(const std::string& str);
  static Sequence* encode(const char* str, size_t n_bases);
  uint8_t* to_chunk(uint8_t n_bases_bytes) const;

  /* Decode the encoded sequence held by this object. */
  std::string decode() const;
};

#endif  // SRC_SEQUENCE_HPP_
