#ifndef ENGINE_WINNOW_HPP_
#define ENGINE_WINNOW_HPP_

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// Underlying type for storing hash values, balancing memory and range
typedef uint16_t Hash;

struct Fingerprint {
    // Hash value generated for a specific k-gram in the input string
    Hash hash;
    // Position within input string corresponding to start of this hash value
    size_t idx;
};

// Length, k, in hashing k-grams for fingerprinting documents
// Minimum length of a substring in which a match can be detected
const uint8_t SUBSTRING_LENGTH = 5;
// Number of sequential hashes within one window
// From each window, at least one hash must be chosen for the fingerprint
const uint8_t WINDOW_SIZE = 4;

/**
 * Generate hashes for each k-gram in the input document.
 */
std::vector<Hash> generate_hashes(const std::string& str) {
    // The input must be at least as long as the length of each k-gram
    if (SUBSTRING_LENGTH > str.length()) {
        std::stringstream ss;
        ss << "Input must span at least one k-gram! (k = "
           << std::to_string(SUBSTRING_LENGTH) << " > "
           << std::to_string(str.length()) << ")";
        throw std::runtime_error(ss.str());
    }

    std::vector<Hash> hashes;
    hashes.reserve(str.length() - SUBSTRING_LENGTH + 1);

    // For each k-gram from a view (does not copy) of the input, record a hash
    std::string_view view = str;
    for (size_t i = 0; i < hashes.capacity(); i++) {
        hashes.push_back(
            std::hash<std::string_view>()(view.substr(i, SUBSTRING_LENGTH)));
    }

    return hashes;
}

/**
 * Create fingerprints for each window of hashes using robust winnowing.
 */
std::vector<Fingerprint> fingerprint_hashes(const std::vector<Hash>& hashes) {
    // The input must yield enough hashes to fill one window
    if (WINDOW_SIZE > hashes.size()) {
        std::stringstream ss;
        ss << "Input must yield enough hashes for one window! (w = "
           << std::to_string(WINDOW_SIZE) << " > "
           << std::to_string(hashes.size()) << ")";
        throw std::runtime_error(ss.str());
    }

    std::vector<Fingerprint> fingerprints;

    size_t curr_idx = 0;
    size_t min_idx = 0;
    Hash min_hash = std::numeric_limits<Hash>::max();

    // First, find the minimum hash (the fingerprint) for the first window
    while (curr_idx < WINDOW_SIZE) {
        if (hashes[curr_idx] <= min_hash) {
            min_idx = curr_idx;
            min_hash = hashes[curr_idx];
        }
        curr_idx++;
    }

    // Add the first trace of the document to its fingerprints!
    fingerprints.push_back(Fingerprint{min_hash, min_idx});

    // Next, find the right-most, minimum hashes for all subsequent windows
    while (curr_idx < hashes.size()) {
        // If the old min is no longer in the window, we must find a new min
        // If cases of ties for the min, we choose the right-most hash
        if (curr_idx - min_idx == WINDOW_SIZE) {
            min_idx = curr_idx;
            min_hash = hashes[curr_idx];
            for (size_t i = curr_idx - 1; i > curr_idx - WINDOW_SIZE; i--) {
                if (hashes[i] < min_hash) {
                    min_idx = i;
                    min_hash = hashes[i];
                }
            }

            // Since this is a newly-found min, it must be a fingerprint
            fingerprints.push_back(Fingerprint{min_hash, min_idx});
        }
        // Otherwise, the old min is in this window, so we have it easy!
        // Now, we need to check if the next hash is a new, right-most min
        else if (hashes[curr_idx] <= min_hash) {
            min_idx = curr_idx;
            min_hash = hashes[curr_idx];

            // Since the right-most hash is a new min, it's also a print
            fingerprints.push_back(Fingerprint{min_hash, min_idx});
        }

        curr_idx++;
    }

    return fingerprints;
}

/**
 * Fingerprint text by applying MOSS's robust winnowing algorithm.
 * http://theory.stanford.edu/~aiken/publications/papers/sigmod03.pdf
 */
std::vector<Fingerprint> winnow(const std::string& input) {
    return fingerprint_hashes(generate_hashes(input));
}

#endif  // ENGINE_WINNOW_HPP_
