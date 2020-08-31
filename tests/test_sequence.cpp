#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "sequence.hpp"

TEST(SequenceTest, test_constructor_with_vector) {
  std::vector<uint8_t> sequence{1, 2, 3};
  Sequence s(sequence, 8);
  const uint8_t* buffer = s.get_sequence();
  EXPECT_THAT(std::vector<uint8_t>(buffer, buffer + s.get_vector_size()),
              ::testing::ElementsAre(1, 2, 3));
  EXPECT_EQ(8, s.get_n_bases());
  EXPECT_EQ(4, s.get_n_blocks());
  EXPECT_EQ(3, s.get_vector_size());
}

TEST(SequenceTest, test_constructor_with_array) {
  uint8_t sequence[] = {1, 2, 3};
  Sequence s(sequence, 8);
  const uint8_t* buffer = s.get_sequence();
  EXPECT_THAT(std::vector<uint8_t>(buffer, buffer + s.get_vector_size()),
              ::testing::ElementsAre(1, 2, 3));
  EXPECT_EQ(8, s.get_n_bases());
  EXPECT_EQ(4, s.get_n_blocks());
  EXPECT_EQ(3, s.get_vector_size());
}

TEST(SequenceTest, test_encode_with_string) {
  std::string str("ACTGCCTA");
  Sequence* s = Sequence::encode(str);
  const uint8_t* buffer = s->get_sequence();
  EXPECT_THAT(std::vector<uint8_t>(buffer, buffer + s->get_vector_size()),
              ::testing::ElementsAre(23, 77, 224));
  EXPECT_EQ(8, s->get_n_bases());
  EXPECT_EQ(4, s->get_n_blocks());
  EXPECT_EQ(3, s->get_vector_size());
  delete s;
}

TEST(SequenceTest, test_encode_with_string_2) {
  std::string str("NTCCTAGTCAGCATGTGACATCGAG");
  Sequence* s = Sequence::encode(str);
  const uint8_t* buffer = s->get_sequence();
  EXPECT_THAT(std::vector<uint8_t>(buffer, buffer + s->get_vector_size()),
              ::testing::ElementsAre(177, 189, 178, 96, 59, 209, 57, 168, 0));
  EXPECT_EQ(25, s->get_n_bases());
  EXPECT_EQ(13, s->get_n_blocks());
  EXPECT_EQ(9, s->get_vector_size());
  delete s;
}

TEST(SequenceTest, test_encode_with_array) {
  char str[] = {'A', 'C', 'T', 'G', 'C', 'C', 'T', 'A'};
  Sequence* s = Sequence::encode(str, 8);
  const uint8_t* buffer = s->get_sequence();
  EXPECT_THAT(std::vector<uint8_t>(buffer, buffer + s->get_vector_size()),
              ::testing::ElementsAre(23, 77, 224));
  EXPECT_EQ(8, s->get_n_bases());
  EXPECT_EQ(4, s->get_n_blocks());
  EXPECT_EQ(3, s->get_vector_size());
  delete s;
}

TEST(SequenceTest, test_decode) {
  std::vector<uint8_t> sequence{23, 77, 224};
  Sequence s(sequence, 8);
  std::string str("ACTGCCTA");
  std::string decoded = s.decode();
  EXPECT_EQ(str, decoded);
}

TEST(SequenceTest, test_decode_2) {
  std::vector<uint8_t> sequence{177, 189, 178, 96, 59, 209, 57, 168, 0};
  Sequence s(sequence, 25);
  std::string str("NTCCTAGTCAGCATGTGACATCGAG");
  std::string decoded = s.decode();
  EXPECT_EQ(str, decoded);
}

TEST(SequenceTest, test_to_chunk) {
  std::vector<uint8_t> sequence{23, 77, 224};
  Sequence s(sequence, 8);
  size_t size = s.get_chunk_size(4);
  uint8_t* chunk = s.to_chunk(4);
  EXPECT_EQ(7, size);
  EXPECT_EQ(0, chunk[0]);
  EXPECT_EQ(0, chunk[1]);
  EXPECT_EQ(0, chunk[2]);
  EXPECT_EQ(8, chunk[3]);
  EXPECT_EQ(23, chunk[4]);
  EXPECT_EQ(77, chunk[5]);
  EXPECT_EQ(224, chunk[6]);
  delete[] chunk;
}

TEST(SequenceTest, test_to_chunk_2) {
  std::vector<uint8_t> sequence{177, 189, 178, 96, 59, 209, 57, 168, 0};
  Sequence s(sequence, 25);
  size_t size = s.get_chunk_size(4);
  uint8_t* chunk = s.to_chunk(4);
  EXPECT_EQ(13, size);
  EXPECT_EQ(0, chunk[0]);
  EXPECT_EQ(0, chunk[1]);
  EXPECT_EQ(0, chunk[2]);
  EXPECT_EQ(25, chunk[3]);
  EXPECT_EQ(177, chunk[4]);
  EXPECT_EQ(189, chunk[5]);
  EXPECT_EQ(178, chunk[6]);
  EXPECT_EQ(96, chunk[7]);
  EXPECT_EQ(59, chunk[8]);
  EXPECT_EQ(209, chunk[9]);
  EXPECT_EQ(57, chunk[10]);
  EXPECT_EQ(168, chunk[11]);
  EXPECT_EQ(0, chunk[12]);
  delete[] chunk;
}
