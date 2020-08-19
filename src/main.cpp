#include <iostream>

#include "reads.hpp"
#include "sequence.hpp"

int main(int argc, char* argv[]) {
  // char test[] = {'A', 'C', 'T', 'G', 'C', 'C', 'T', 'A'};
  // Sequence s = Sequence::encode(test, 8);
  // for (size_t i = 0; i < s.sequence.size(); i++) {
  //   std::cout << unsigned(s.sequence.at(i)) << std::endl;
  // }

  // uint8_t test[] = {47, 70, 241};
  // std::string filename("filename");
  // Sequence s(test, 3, 8);
  // Reads r(filename, Mode::Write);
  // r.add_sequence(s);
  // r.write_chunks();

  // std::string filename("testfile.lfq");
  // Reads r(filename, Mode::Read);
  // Sequence* s = r.read_sequence_chunk();
  // std::cout << s->decode() << std::endl;

  return 0;
}
