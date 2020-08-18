#ifndef SRC_SEQUENCE_HPP_
#define SRC_SEQUENCE_HPP_
#include <cmath>
#include <string>
#include <vector>

struct Sequence {
  /* Static constants related to block size and base encodings. */
  static const size_t BLOCK_SIZE;
  static const uint8_t MASK;
  static const uint8_t BASE_N;
  static const uint8_t BASE_A;
  static const uint8_t BASE_T;
  static const uint8_t BASE_C;
  static const uint8_t BASE_G;

  /* The actual encoded sequence & number of (encoded) bases */
  std::vector<uint8_t> sequence;
  size_t n_bases;

  /* Constructors. */
  Sequence();
  Sequence(const std::vector<uint8_t>& sequence, size_t n_bases);
  Sequence(const uint8_t* sequence, size_t size, size_t n_bases);

  /* Static methods to convert a char to its encoding and vice-versa. */
  static uint8_t char_to_encoding(const char c);
  static char encoding_to_char(const uint8_t e);

  /* Static "constructor" methods that creates a Sequence object with the
   * provided string/char array encoded.
   */
  static Sequence& encode(const std::string& str);
  static Sequence& encode(const char* str, size_t size);

  /* Decode the encoded sequence held by this object. */
  char* decode();
};

#endif  // SRC_SEQUENCE_HPP_
