#ifndef SRC_SEQUENCE_HPP_
#define SRC_SEQUENCE_HPP_
#include <cmath>
#include <string>
#include <vector>

class Sequence {
 private:
  /* Static constants related base encodings. */
  static const uint8_t MASK;
  static const uint8_t BASE_N;
  static const uint8_t BASE_A;
  static const uint8_t BASE_T;
  static const uint8_t BASE_C;
  static const uint8_t BASE_G;

  /* The actual encoded sequence & number of (encoded) bases */
  std::vector<uint8_t> sequence;
  size_t n_bases;

  /* Private empty constructor. */
  Sequence();

  /* Static methods to convert a char to its encoding and vice-versa. */
  static uint8_t char_to_encoding(const char c);
  static char encoding_to_char(const uint8_t e);

 public:
  /* Other static constants, but these can be accessed from other classes. */
  static const size_t BLOCK_SIZE;
  static const size_t TYPE_SIZE;

  /* Constructors. */
  Sequence(const std::vector<uint8_t>& sequence, size_t n_bases);
  Sequence(const uint8_t* sequence, size_t size, size_t n_bases);

  /* Accessors */
  const std::vector<uint8_t> get_sequence() const;
  size_t get_n_bases() const;
  static size_t get_type_size();

  /* Static "constructor" methods that creates a Sequence object with the
   * provided string/char array encoded.
   */
  static Sequence* encode(const std::string& str);
  static Sequence* encode(const char* str, size_t n_bases);
  uint8_t* to_chunk(uint8_t n_bases_bytes, size_t* size) const;

  /* Decode the encoded sequence held by this object. */
  std::string decode() const;
};

#endif  // SRC_SEQUENCE_HPP_
