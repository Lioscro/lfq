#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stream.hpp"

TEST(StreamTest, test_instream_plain) {
  std::string filename("../tests/fixtures/test.txt");
  InStream in(filename);
  EXPECT_EQ(StreamType::Plain, in.get_type());
  std::string line;
  getline(in.get_stream(), line);
  EXPECT_EQ("test", line);
}

TEST(StreamTest, test_instream_gzip) {
  std::string filename("../tests/fixtures/test.txt.gz");
  InStream in(filename);
  EXPECT_EQ(StreamType::Gzip, in.get_type());
  std::string line;
  getline(in.get_stream(), line);
  EXPECT_EQ("test", line);
}
