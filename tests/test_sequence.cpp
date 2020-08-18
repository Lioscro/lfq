#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "sequence.hpp"

TEST(SequenceTest, test_constructor) { Sequence s; }

TEST(SequenceTest, test_constructor_with_vector) {
  std::vector<uint8_t> sequence{1, 2, 3};
  Sequence s(sequence, 4);
  ASSERT_THAT(s.sequence, ::testing::ElementsAre(1, 2, 3));
  ASSERT_EQ(4, s.n_bases);
}
