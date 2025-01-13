#include <iostream>
#include <vector>
#include <unordered_set>

namespace SJF
{

class PermutationGenerator {
public:
    // Constructor to initialize N_ and K_
    PermutationGenerator(int n, int k) : N_(n), K_(k) {}

    // Generates all unique permutations of the first K elements
    const std::vector<std::vector<int>> generatePermutations() {
        if(N_ == 0 || K_ == 0 || K_ > N_) {
            return {};
        }
        permutations_.clear(); // Clear previous permutations
        current_.clear(); // Clear the current permutation
        std::unordered_set<int> used_; // Tracks used numbers
        backtrack(used_); // Start the recursive process
        return permutations_;
    }

    // Returns the generated permutations
    const std::vector<std::vector<int>> getPermutations() const {
        return permutations_;
    }

    // Prints all permutations
    void printPermutations() const {
        for (const auto& perm : permutations_) {
            for (int num : perm) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }
private:
    int N_; // Range of numbers: 0 to N-1
    int K_; // Number of elements to pick (first K elements)
    std::vector<std::vector<int>> permutations_; // Stores all valid permutations
    std::vector<int> current_; // Current permutation being built

    // Recursive backtracking function to generate permutations
    void backtrack(std::unordered_set<int>& used_) {
        // If the current permutation has K elements, add it to the result
        if (current_.size() == K_) {
            permutations_.push_back(current_);
            return;
        }

        // Try all numbers from 0 to N-1
        for (int i = 0; i < N_; ++i) {
            // If the number i is not used in the current permutation
            if (used_.find(i) == used_.end()) {
                used_.insert(i); // Mark i as used
                current_.push_back(i); // Add i to the current permutation
                backtrack(used_); // Recursively generate the next number
                current_.pop_back(); // Backtrack: remove i from the current permutation
                used_.erase(i); // Unmark i as used
            }
        }
    }
};
}