#include "engine/winnow.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

std::string sample_strings[] = {"Hello, world!", "Hello, wonder.",
                                "Science is fun", "Pokemon is fun"};

void print_sample_hashes() {
    for (const std::string& str : sample_strings) {
        std::vector<Hash> hashes = generate_hashes(str);
        std::string hashes_str;
        for (Hash h : hashes) {
            std::stringstream ss;
            ss << std::setw(5) << std::setfill('0') << h;
            hashes_str += ss.str() + " ";
        }

        std::cout << "String: " << str << "\n";
        std::cout << "Hashes: " << hashes_str << "\n" << std::endl;
    }
}

void print_sample_fingerprints() {
    for (const std::string& str : sample_strings) {
        std::vector<Hash> hashes = generate_hashes(str);
        std::vector<Fingerprint> fingerprints = fingerprint_hashes(hashes);
        std::string fingerprints_str;
        for (Fingerprint f : fingerprints) {
            std::stringstream ss;
            ss << "[" << std::setw(5) << std::setfill('0') << f.hash << " "
               << f.idx << "]";
            fingerprints_str += ss.str() + " ";
        }

        std::cout << "String: " << str << "\n";
        std::cout << "Prints: " << fingerprints_str << "\n" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // If the user gives passes no arguments, get an input string for winnowing
    if (argc == 1) {
        std::string input;
        std::cout << "Enter string to be fingerprinted: ";
        std::getline(std::cin, input);
        std::cout << std::endl;

        std::vector<Fingerprint> fingerprints = winnow(input);
        std::cout << "Hash | Position";
        for (Fingerprint f : fingerprints) {
            std::stringstream ss;
            ss << std::setw(5) << std::setfill('0') << f.hash << ", " << f.idx;
            std::cout << "\n" << ss.str();
        }

        std::cout << "\n" << std::endl;
    }

    // If the user gives two arguments (input and output file), fingerprint the
    // input file and write the winnowing results to the output
    else if (argc == 3) {
        std::ifstream ifs(argv[1]);
        std::string input((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));

        std::vector<Fingerprint> fingerprints = winnow(input);

        std::ofstream file(argv[2]);
        if (file.is_open()) {
            for (Fingerprint f : fingerprints) {
                std::stringstream ss;
                ss << std::setw(5) << std::setfill('0') << f.hash << ", "
                   << f.idx;
                file << ss.str() << "\n";
            }

            file.close();
        }
    }

    // In all other cases, simply print sample sentence hashes and fingerprints
    else {
        print_sample_hashes();
        print_sample_fingerprints();
    }

    return 0;
}
