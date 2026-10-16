
#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>

class CryptoLab {
private:
    std::mt19937 rng{std::random_device{}()};

    static constexpr const char* ALPHABET =
        "abcdefghijklmnopqrstuvwxyz ";

    static int indexOf(char c) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }

        for (int i = 0; i < 27; ++i) {
            if (ALPHABET[i] == c) {
                return i;
            }
        }

        throw std::invalid_argument(
            "Only English letters and spaces are supported."
        );
    }

public:
    std::array<char, 26> generateSubstitutionKey() {
        std::array<char, 26> key{};
        std::iota(key.begin(), key.end(), 'A');
        std::shuffle(key.begin(), key.end(), rng);
        return key;
    }

    std::string substitute(
        const std::string& text,
        const std::array<char, 26>& key
    ) {
        std::string result;

        for (unsigned char c : text) {
            if (c >= 'A' && c <= 'Z') {
                result += key[c - 'A'];
            } else if (c >= 'a' && c <= 'z') {
                result += static_cast<char>(
                    std::tolower(
                        static_cast<unsigned char>(key[c - 'a'])
                    )
                );
            } else {
                result += static_cast<char>(c);
            }
        }

        return result;
    }

    std::string frequencyAttack(const std::string& ciphertext) {
        std::array<int, 26> frequency{};

        for (unsigned char c : ciphertext) {
            if (c >= 'A' && c <= 'Z') {
                ++frequency[c - 'A'];
            } else if (c >= 'a' && c <= 'z') {
                ++frequency[c - 'a'];
            }
        }

        const std::string englishFrequency =
            "ETAOINSHRDLCUMWFGYPBVKJXQZ";

        std::array<int, 26> letters{};
        std::iota(letters.begin(), letters.end(), 0);

        std::sort(
            letters.begin(),
            letters.end(),
            [&](int a, int b) {
                return frequency[a] > frequency[b];
            }
        );

        std::array<char, 26> guessedKey{};

        for (int i = 0; i < 26; ++i) {
            guessedKey[letters[i]] = englishFrequency[i];
        }

        std::string result;

        for (unsigned char c : ciphertext) {
            if (c >= 'A' && c <= 'Z') {
                result += guessedKey[c - 'A'];
            } else if (c >= 'a' && c <= 'z') {
                result += static_cast<char>(
                    std::tolower(
                        static_cast<unsigned char>(
                            guessedKey[c - 'a']
                        )
                    )
                );
            } else {
                result += static_cast<char>(c);
            }
        }

        return result;
    }

    // Encrypt using the 27-character alphabet.
    std::string encryptOTP27(
        const std::string& plaintext,
        const std::string& key
    ) {
        if (plaintext.size() != key.size()) {
            throw std::invalid_argument(
                "Plaintext and key must have equal lengths."
            );
        }

        std::string ciphertext;
        ciphertext.reserve(plaintext.size());

        for (std::size_t i = 0; i < plaintext.size(); ++i) {
            const int m = indexOf(plaintext[i]);
            const int k = indexOf(key[i]);

            ciphertext += ALPHABET[(m + k) % 27];
        }

        return ciphertext;
    }

    std::string decryptOTP27(
        const std::string& ciphertext,
        const std::string& key
    ) {
        if (ciphertext.size() != key.size()) {
            throw std::invalid_argument(
                "Ciphertext and key must have equal lengths."
            );
        }

        std::string plaintext;
        plaintext.reserve(ciphertext.size());

        for (std::size_t i = 0; i < ciphertext.size(); ++i) {
            const int c = indexOf(ciphertext[i]);
            const int k = indexOf(key[i]);

            plaintext += ALPHABET[(c - k + 27) % 27];
        }

        return plaintext;
    }

    std::string deriveKey(
        const std::string& ciphertext,
        const std::string& plaintext
    ) {
        if (ciphertext.size() != plaintext.size()) {
            throw std::invalid_argument(
                "Ciphertext and plaintext must have equal lengths."
            );
        }

        std::string key;
        key.reserve(ciphertext.size());

        for (std::size_t i = 0; i < ciphertext.size(); ++i) {
            const int c = indexOf(ciphertext[i]);
            const int m = indexOf(plaintext[i]);

            key += ALPHABET[(c - m + 27) % 27];
        }

        return key;
    }
};

void printBanner() {
    std::cout <<
        "======================================\n"
        "        ██████╗ ████████╗██████╗    \n"
        "       ██╔═══██╗╚══██╔══╝██╔══██╗\n"
        "       ██║   ██║   ██║   ██████╔╝\n"
        "       ██║   ██║   ██║   ██╔═══╝ \n"
        "       ╚██████╔╝   ██║   ██║     \n"
        "        ╚═════╝    ╚═╝   ╚═╝  \n"
        "           ONE TIME PAD LAB\n"
        "=======================================\n\n";
}

void runSubstitutionExperiment(CryptoLab& lab) {
    std::string message;

    std::cout << "\n[SUBSTITUTION CIPHER]\n";
    std::cout << "Enter an English message: ";
    std::getline(std::cin, message);

    const auto key = lab.generateSubstitutionKey();
    const std::string ciphertext = lab.substitute(message, key);
    const std::string guess = lab.frequencyAttack(ciphertext);

    std::cout << "Ciphertext: " << ciphertext << '\n';
    std::cout << "Frequency attack guess: " << guess << '\n';
}

void runOTPExample(CryptoLab& lab) {
    const std::string ciphertext =
        "ankyodkyurepfjbyojdsplreyiunofdoiu erfpluyts";

    const std::string plaintext1 =
        "mr mustard with the candlestick in the hall";

    const std::string plaintext2 =
        "miss scarlet with the knife in the library";

    // Derive keys that are mathematically consistent with
    // the ciphertext and each intended plaintext.
    const std::string key1 =
        lab.deriveKey(ciphertext, plaintext1);

    const std::string key2 =
        lab.deriveKey(ciphertext, plaintext2);

    std::cout << "\n[ONE-TIME PAD DEMONSTRATION]\n\n";
    std::cout << "Ciphertext: " << ciphertext << "\n\n";

    std::cout << "Key 1: " << key1 << '\n';
    std::cout << "Plaintext 1: "
              << lab.decryptOTP27(ciphertext, key1) << "\n\n";

    std::cout << "Key 2: " << key2 << '\n';
    std::cout << "Plaintext 2: "
              << lab.decryptOTP27(ciphertext, key2) << '\n';
}

int main() {
    try {
        CryptoLab lab;
        printBanner();

        std::cout << "1. Substitution cipher + frequency attack\n";
        std::cout << "2. One-Time Pad demonstration\n";
        std::cout << "3. Run both experiments\n";
        std::cout << "\nSelect an option: ";

        int option = 0;

        if (!(std::cin >> option)) {
            std::cerr << "Invalid option.\n";
            return 1;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        if (option == 1 || option == 3) {
            runSubstitutionExperiment(lab);
        }

        if (option == 2 || option == 3) {
            runOTPExample(lab);
        }

        if (option < 1 || option > 3) {
            std::cerr << "Unknown option.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
