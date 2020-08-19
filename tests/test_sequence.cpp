#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "sequence.hpp"

TEST(SequenceTest, test_constructor_with_vector) {
  std::vector<uint8_t> sequence{1, 2, 3};
  Sequence s(sequence, 4);
  EXPECT_THAT(s.get_sequence(), ::testing::ElementsAre(1, 2, 3));
  EXPECT_EQ(4, s.get_n_bases());
}

TEST(SequenceTest, test_constructor_with_array) {
  uint8_t sequence[] = {1, 2, 3};
  Sequence s(sequence, 3, 4);
  EXPECT_THAT(s.get_sequence(), ::testing::ElementsAre(1, 2, 3));
  EXPECT_EQ(4, s.get_n_bases());
}

TEST(SequenceTest, test_encode_with_string) {
  std::string str("ACTGCCTA");
  Sequence* s = Sequence::encode(str);
  EXPECT_THAT(s->get_sequence(), ::testing::ElementsAre(47, 70, 241));
  EXPECT_EQ(8, s->get_n_bases());
  delete s;
}

TEST(SequenceTest, test_encode_with_array) {
  char str[] = {'A', 'C', 'T', 'G', 'C', 'C', 'T', 'A'};
  Sequence* s = Sequence::encode(str, 8);
  EXPECT_THAT(s->get_sequence(), ::testing::ElementsAre(47, 70, 241));
  EXPECT_EQ(8, s->get_n_bases());
  delete s;
}

TEST(SequenceTest, test_decode) {
  std::vector<uint8_t> sequence{47, 70, 241};
  Sequence s(sequence, 8);
  std::string str("ACTGCCTA");
  uint8_t* decoded = s.decode();
  EXPECT_STREQ(str.c_str(), (char*)decoded);
  delete[] decoded;
}

TEST(SequenceTest, test_to_chunk) {
  size_t size;
  std::vector<uint8_t> sequence{47, 70, 241};
  Sequence s(sequence, 9999);
  uint8_t* chunk = s.to_chunk(4, &size);
  EXPECT_EQ(7, size);
  EXPECT_EQ(0, chunk[0]);
  EXPECT_EQ(0, chunk[1]);
  EXPECT_EQ(39, chunk[2]);
  EXPECT_EQ(15, chunk[3]);
  EXPECT_EQ(47, chunk[4]);
  EXPECT_EQ(70, chunk[5]);
  EXPECT_EQ(241, chunk[6]);
  delete[] chunk;
}
