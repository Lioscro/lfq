#include "reads.hpp"

const char Reads::EXTENSION[] = "lfq";
const char Reads::INDEX_EXTENSION[] = "lfqi";
const uint8_t Reads::BASES_BYTES = 4;

Reads::Reads(const std::string& path, ReadsMode mode)
    : file(), index(), read_i(0) {
  this->open(path, mode);
}

Reads::~Reads() { this->file.close(); }

const std::vector<uint32_t> Reads::get_index() const { return this->index; }

size_t Reads::get_pos() { return this->file.tellg(); }

size_t Reads::get_read_i() { return this->read_i; }

void Reads::open(const std::string& path, ReadsMode mode) {
  switch (mode) {
    case ReadsMode::Read:
      this->file.open(path, std::fstream::in | std::fstream::binary);
      break;
    case ReadsMode::Write:
      this->file.open(
          path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
      break;
    default:
      throw std::runtime_error(std::string("Unknown file mode: ") +
                               std::to_string(mode));
  }

  if (!this->file.is_open()) {
    throw std::runtime_error("Bad stream");
  }
}

size_t Reads::read_n_bases() {
  size_t n_bases = 0;
  char* n_bases_buffer = new char[this->BASES_BYTES]();
  this->file.read(n_bases_buffer, this->BASES_BYTES);
  if (this->file.eof()) {
    throw std::runtime_error("Malformed n_bases");
  }
  for (uint8_t i = 0; i < this->BASES_BYTES; i++) {
    n_bases += ((size_t)n_bases_buffer[i])
               << (Sequence::TYPE_SIZE * (this->BASES_BYTES - i - 1));
  }
  delete[] n_bases_buffer;
  return n_bases;
}

char* Reads::read_sequence(size_t vector_size) {
  char* buffer = new char[vector_size]();
  this->file.read(buffer, vector_size);
  if (this->file.eof()) {
    delete[] buffer;
    throw std::runtime_error("Malformed sequence");
  }
  return buffer;
}

void Reads::write_sequence_chunk(const Sequence& sequence) {
  uint32_t old_pos = this->file.tellg();
  size_t size = sequence.get_chunk_size(this->BASES_BYTES);
  uint8_t* chunk = sequence.to_chunk(this->BASES_BYTES);
  this->file.write(reinterpret_cast<char*>(chunk), size);
  delete[] chunk;
  this->index.push_back(old_pos);
  this->read_i++;
}

Sequence* Reads::read_sequence_chunk() {
  // Check index integrity.
  if (this->index.size() < this->read_i) {
    throw std::runtime_error("Failed index integrity check");
  }
  if (this->file.peek() == EOF) {
    return nullptr;
  }

  uint32_t old_pos = this->file.tellg();
  size_t n_bases = this->read_n_bases();
  size_t n_blocks = Sequence::get_n_blocks(n_bases);
  size_t vector_size = Sequence::get_vector_size(n_blocks);
  char* buffer = this->read_sequence(vector_size);
  Sequence* s = new Sequence(reinterpret_cast<uint8_t*>(buffer), n_bases);
  delete[] buffer;

  // Add to index if this read is valid (i.e. if code gets here)
  if (this->index.size() == this->read_i) {
    this->index.push_back(old_pos);
  }
  this->read_i++;

  return s;
}

void Reads::write_index(const std::string& path) {
  OutStream stream(
      path, is_gzip_extension(path) ? StreamType::Gzip : StreamType::Plain);
  for (size_t i = 0; i < this->index.size(); i++) {
    stream.get_stream() << std::to_string(this->index[i]) << std::endl;
  }
}

void Reads::read_index(const std::string& path) {
  std::string line;
  InStream stream(path);
  while (getline(stream.get_stream(), line)) {
    this->index.push_back(std::stoul(line));
  }
}

void Reads::read_index(const std::string& path, size_t n) {
  std::string line;
  InStream stream(path);
  for (size_t i = 0; i < n; i++) {
    if (getline(stream.get_stream(), line)) {
      this->index.push_back(std::stoul(line));
    } else {
      throw std::runtime_error("Reached end of index");
    }
  }
}

void Reads::build_index() {
  uint32_t pos = 0;
  uint32_t old_pos = this->file.tellg();
  size_t n_bases;
  size_t n_blocks;
  size_t vector_size;
  this->index.clear();
  this->file.seekg(pos);

  while (true) {
    pos = this->file.tellg();
    n_bases = this->read_n_bases();
    n_blocks = Sequence::get_n_blocks(n_bases);
    vector_size = Sequence::get_vector_size(n_blocks);
    this->file.seekg(vector_size, std::istream::cur);
    this->index.push_back(pos);

    if (this->file.peek() == EOF) {
      this->file.clear();
      this->file.seekg(old_pos);
      return;
    }
  }
}

void Reads::seek(size_t i) {
  // Check if location exists in index.
  if (i < this->index.size()) {
    this->file.seekg(this->index[i]);
    return;
  }

  // Otherwise, jump through the file, starting at the last position, to find
  // it.
  size_t current_i = 0;
  uint32_t pos = 0;
  size_t n_bases;
  size_t vector_size;
  if (this->index.size() > 0) {
    current_i = this->index.size() - 1;
    pos = this->index[current_i];
    this->file.seekg(pos);
    n_bases = this->read_n_bases();
    vector_size = Sequence::get_vector_size(Sequence::get_n_blocks(n_bases));
    this->file.seekg(vector_size, std::istream::cur);
  } else {
    this->file.seekg(0);
  }

  while (current_i < i) {
    if (this->file.peek() == EOF) {
      throw std::runtime_error("Reached EOF while seeking");
    }

    pos = this->file.tellg();
    n_bases = this->read_n_bases();
    vector_size = Sequence::get_vector_size(Sequence::get_n_blocks(n_bases));
    this->file.seekg(vector_size, std::istream::cur);
    this->index.push_back(pos);
    current_i++;
  }
  this->read_i = i;
}
