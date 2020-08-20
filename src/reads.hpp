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
  /* Defines how many bytes will be used to encode the number of bases in
   * a read.
   */
  static const uint8_t BASES_BYTES;

  /* File object */
  std::string filename;
  std::fstream file;
  void open(const std::string& filename, Mode mode);

  /* Index for fast seeking. read_i holds the read index of the NEXT read to
   * read/write .*/
  std::vector<uint32_t> index;
  size_t read_i;

  /* Read the first BASES_BYTES bytes from a chunk and decode those bytes into
   * a size_t number, indicating how many bases are in the sequence.
   */
  size_t read_n_bases();

  /*
   * Read the sequence from the encoded sequence in the chunk.
   * vector_size is the number of bytes used to encode the sequence.
   * Must be called after read_n_bases.
   */
  char* read_sequence(size_t vector_size);

 public:
  static const char EXTENSION[];
  static const char INDEX_EXTENSION[];

  Reads(const std::string& filename, Mode mode);
  ~Reads();

  /* Accessors */
  const std::vector<uint32_t> get_index() const;
  size_t get_pos();
  size_t get_read_i();

  /* Functions to write and read a sequence object from the file. */
  void write_sequence_chunk(const Sequence& sequence);
  Sequence* read_sequence_chunk();

  /* Index stuff */
  void write_index(const std::string& filename);
  void read_index(const std::string& filename);
  void build_index();  // Only in Mode::Read

  /* Function to seek to the start of a specific read. */
  void seek(size_t i);
};

#endif  // SRC_READS_HPP_
