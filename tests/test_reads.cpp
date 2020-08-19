#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "reads.hpp"
#include "sequence.hpp"

TEST(ReadsTest, test_read_sequence_chunk) {
  std::string filename("../tests/fixtures/testfile.lfq");
  Reads r(filename, Mode::Read);
  Sequence* s = r.read_sequence_chunk();
  std::string str("ACTGCCTA");
  uint8_t* decoded = s->decode();
  EXPECT_STREQ(str.c_str(), reinterpret_cast<char*>(decoded));
  delete[] decoded;
}
