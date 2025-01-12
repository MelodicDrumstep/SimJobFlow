#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>
#include "permutation_generator.hpp" // Include the header file for PermutationGenerator

using namespace SJF;

// Test fixture for PermutationGenerator
class PermutationsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test case 1: N = 3, K = 2
TEST_F(PermutationsTest, N3K2) {
    PermutationGenerator generator(3, 2);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=3, K=2
    std::vector<std::vector<int>> expected = {
        {0, 1},
        {0, 2},
        {1, 0},
        {1, 2},
        {2, 0},
        {2, 1}
    };

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());

    // Check each permutation
    for (const auto& perm : expected) {
        EXPECT_NE(std::find(permutations.begin(), permutations.end(), perm), permutations.end());
    }
}

// Test case 2: N = 4, K = 1
TEST_F(PermutationsTest, N4K1) {
    PermutationGenerator generator(4, 1);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=4, K=1
    std::vector<std::vector<int>> expected = {
        {0},
        {1},
        {2},
        {3}
    };

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());

    // Check each permutation
    for (const auto& perm : expected) {
        EXPECT_NE(std::find(permutations.begin(), permutations.end(), perm), permutations.end());
    }
}

// Test case 3: N = 2, K = 2
TEST_F(PermutationsTest, N2K2) {
    PermutationGenerator generator(2, 2);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=2, K=2
    std::vector<std::vector<int>> expected = {
        {0, 1},
        {1, 0}
    };

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());

    // Check each permutation
    for (const auto& perm : expected) {
        EXPECT_NE(std::find(permutations.begin(), permutations.end(), perm), permutations.end());
    }
}

// Test case 4: N = 1, K = 1
TEST_F(PermutationsTest, N1K1) {
    PermutationGenerator generator(1, 1);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=1, K=1
    std::vector<std::vector<int>> expected = {
        {0}
    };

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());

    // Check each permutation
    for (const auto& perm : expected) {
        EXPECT_NE(std::find(permutations.begin(), permutations.end(), perm), permutations.end());
    }
}

// Test case 5: N = 0, K = 0 (edge case)
TEST_F(PermutationsTest, N0K0) {
    PermutationGenerator generator(0, 0);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=0, K=0
    std::vector<std::vector<int>> expected = {};

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());
}

// Test case 6: N = 3, K = 3 (full permutations)
TEST_F(PermutationsTest, N3K3) {
    PermutationGenerator generator(3, 3);
    auto permutations = generator.generatePermutations();

    // Expected permutations for N=3, K=3
    std::vector<std::vector<int>> expected = {
        {0, 1, 2},
        {0, 2, 1},
        {1, 0, 2},
        {1, 2, 0},
        {2, 0, 1},
        {2, 1, 0}
    };

    // Check the number of permutations
    EXPECT_EQ(permutations.size(), expected.size());

    // Check each permutation
    for (const auto& perm : expected) {
        EXPECT_NE(std::find(permutations.begin(), permutations.end(), perm), permutations.end());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}