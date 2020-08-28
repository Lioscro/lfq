#include "stream.hpp"

bool is_gzip(const std::string& path) {
  // Gzip files have fixed two first bytes.
  std::ifstream file(path, std::ifstream::binary);
  char buffer[2];
  file.read(buffer, 2);
  file.close();
  return ((buffer[0] == static_cast<char>(0x1f)) &&
          (buffer[1] == static_cast<char>(0x8b)));
}

bool is_gzip_extension(const std::string& path) {
  return boost::ends_with(path, ".gz");
}

InStream::InStream() : stream(&std::cin), type(StreamType::Std) {}

InStream::InStream(const std::string& path) {
  if (is_gzip(path)) {
    this->type = StreamType::Gzip;
    this->file.open(path, std::ifstream::binary);
    this->decompressor.push(boost::iostreams::gzip_decompressor());
    this->decompressor.push(this->file);
    this->stream = &this->decompressor;
  } else {
    this->type = StreamType::Plain;
    this->file.open(path);
    this->stream = &this->file;
  }

  if (!this->file.is_open()) {
    throw std::runtime_error("Failed to open InStream");
  }
}

InStream::~InStream() {
  if (this->file.is_open()) {
    this->file.close();
  }
}

StreamType InStream::get_type() { return this->type; }

std::istream& InStream::get_stream() { return *(this->stream); }

OutStream::OutStream()
    : stream(&std::cout), file(), compressor(), type(StreamType::Std) {}

OutStream::OutStream(const std::string& path, StreamType type)
    : stream(nullptr), file(), compressor(), type(type) {
  switch (this->type) {
    case StreamType::Gzip:
      this->file.open(path, std::ofstream::binary | std::ofstream::trunc);
      this->compressor.push(
          boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(
              boost::iostreams::gzip::best_compression)));
      this->compressor.push(this->file);
      this->stream = &this->compressor;
      break;
    case StreamType::Plain:
      this->file.open(path, std::ofstream::trunc);
      this->stream = &this->file;
      break;
    default:
      throw std::runtime_error("Unknown StreamType");
  }
}

OutStream::~OutStream() {
  boost::iostreams::close(this->compressor);
  if (this->file.is_open()) {
    this->file.close();
  }
}

StreamType OutStream::get_type() { return this->type; }

std::ostream& OutStream::get_stream() { return *(this->stream); }
