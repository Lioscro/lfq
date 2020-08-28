#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "reads.hpp"
#include "sequence.hpp"

TEST(ReadsTest, test_read_sequence_chunk) {
  std::string filename("../tests/fixtures/testfile.lfq");
  Reads r(filename, ReadsMode::Read);
  Sequence* s1 = r.read_sequence_chunk();
  std::string str1("ACTGCCTA");
  std::string decoded1 = s1->decode();
  EXPECT_EQ(str1, decoded1);
  delete s1;

  Sequence* s2 = r.read_sequence_chunk();
  std::string str2("CNAC");
  std::string decoded2 = s2->decode();
  EXPECT_EQ(str2, decoded2);
  delete s2;

  // There are only two sequences.
  Sequence* s3 = r.read_sequence_chunk();
  EXPECT_EQ(nullptr, s3);
}

TEST(ReadsTest, test_read_index) {
  std::string filename("../tests/fixtures/testfile.lfq");
  std::string index_filename("../tests/fixtures/testfile.lfqi");
  Reads r(filename, ReadsMode::Read);
  r.read_index(index_filename);
  EXPECT_THAT(r.get_index(), ::testing::ElementsAre(0, 7));
}

TEST(ReadsTest, test_read_index_with_n) {
  std::string filename("../tests/fixtures/testfile.lfq");
  std::string index_filename("../tests/fixtures/testfile.lfqi");
  Reads r(filename, Mode::Read);
  r.read_index(index_filename, 1);
  EXPECT_THAT(r.get_index(), ::testing::ElementsAre(0));
}

TEST(ReadsTest, test_build_index) {
  std::string filename("../tests/fixtures/testfile.lfq");
  Reads r(filename, ReadsMode::Read);
  r.build_index();
  EXPECT_THAT(r.get_index(), ::testing::ElementsAre(0, 7));
  EXPECT_EQ(0, r.get_pos());
}

TEST(ReadsTest, test_seek_uses_index) {
  std::string filename("../tests/fixtures/testfile.lfq");
  std::string index_filename("../tests/fixtures/testfile.lfqi");
  Reads r(filename, ReadsMode::Read);
  r.read_index(index_filename);
  r.seek(1);
  EXPECT_EQ(7, r.get_pos());

  Sequence* s = r.read_sequence_chunk();
  std::string str("CNAC");
  std::string decoded = s->decode();
  EXPECT_EQ(str, decoded);
  delete s;
}

TEST(ReadsTest, test_seek_jumps) {
  std::string filename("../tests/fixtures/testfile.lfq");
  Reads r(filename, ReadsMode::Read);
  r.seek(1);
  EXPECT_EQ(7, r.get_pos());
  EXPECT_THAT(r.get_index(), ::testing::ElementsAre(0));

  Sequence* s = r.read_sequence_chunk();
  std::string str("CNAC");
  std::string decoded = s->decode();
  EXPECT_EQ(str, decoded);
  EXPECT_THAT(r.get_index(), ::testing::ElementsAre(0, 7));
  delete s;
}
