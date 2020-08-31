#ifndef SRC_STREAM_HPP_
#define SRC_STREAM_HPP_
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "boost/algorithm/string/predicate.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "boost/iostreams/filtering_stream.hpp"

bool is_gzip(const std::string& path);
bool is_gzip_extension(const std::string& path);
std::istream& getline(std::istream& is, std::string& str);  // NOLINT

enum StreamType { Std, Plain, Gzip };

class InStream {
 private:
  std::istream* stream;
  std::ifstream file;
  boost::iostreams::filtering_istream decompressor;
  StreamType type;

 public:
  /* Stdin */
  InStream();
  /* Plain or gzip file */
  explicit InStream(const std::string& path);
  ~InStream();

  /* Accessors */
  StreamType get_type();
  std::istream& get_stream();
};

class OutStream {
 private:
  std::ostream* stream;
  std::ofstream file;
  boost::iostreams::filtering_ostream compressor;
  StreamType type;

 public:
  /* Stdout */
  OutStream();
  /* Plain or gzip file */
  OutStream(const std::string& path, StreamType type);
  ~OutStream();

  /* Accessors */
  StreamType get_type();
  std::ostream& get_stream();
};

#endif  // SRC_STREAM_HPP_
