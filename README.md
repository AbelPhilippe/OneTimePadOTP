# One-Time Pad Lab

A C++ cryptography lab demonstrating the One-Time Pad (OTP), a classical substitution cipher, and a basic frequency-analysis attack.

The project explores a central idea in cryptography: **the same ciphertext can correspond to different plaintexts when different keys are used**. It also illustrates why a correctly implemented One-Time Pad has information-theoretic, or perfect, secrecy.

## Table of Contents

- [Project Overview](#project-overview)
- [Historical Background](#historical-background)
- [How the Program Works](#how-the-program-works)
- [The 27-Character OTP Demonstration](#the-27-character-otp-demonstration)
- [Why the One-Time Pad Is Unbreakable in Theory](#why-the-one-time-pad-is-unbreakable-in-theory)
- [Requirements for Perfect Secrecy](#requirements-for-perfect-secrecy)
- [Substitution Cipher and Frequency Analysis](#substitution-cipher-and-frequency-analysis)
- [Build and Run](#build-and-run)
- [Limitations and Security Notes](#limitations-and-security-notes)
- [References](#references)

## Project Overview

One-Time Pad Lab is an educational C++ program with two experiments:

1. **Substitution cipher:** generates a random monoalphabetic substitution key, encrypts a message, and applies a simple frequency-analysis heuristic to the ciphertext.
2. **One-Time Pad demonstration:** uses an alphabet containing the 26 English letters plus a space to show how the same ciphertext can decrypt to two different messages under two different keys.

The project is intended for learning and experimentation. It is not a production cryptographic application.

## Historical Background

The One-Time Pad grew out of work on telegraph-era encryption and the Vernam cipher. In 1917, Gilbert S. Vernam developed a cipher system for teleprinter communications. His original system used a key stream; security depended on the properties of that key stream.

Joseph O. Mauborgne, an officer in the U.S. Army Signal Corps, recognized the importance of using a genuinely random key that was as long as the message and never reused. This improvement became known as the **One-Time Pad**.

The defining rule is simple:

- The key must be truly random.
- It must be at least as long as the message.
- It must remain secret.
- It must be used for one message only.
- It must never be reused.

Claude Shannon later established the theoretical foundations of perfect secrecy, showing how a cipher such as the correctly used One-Time Pad can provide information-theoretic security.

## How the Program Works

### 1. Substitution cipher

The program can generate a shuffled mapping of the 26 English letters. Each plaintext letter is replaced by the corresponding letter in that mapping.

For example, one randomly generated mapping might replace `A` with `Q`, `B` with `M`, and so on. The actual mapping changes between runs.

The program then counts the frequency of letters in the ciphertext and maps the most frequent ciphertext letters to common English letters. This is a basic heuristic, not a complete cryptanalysis system. It may perform poorly on short messages or text that does not follow typical English letter frequencies.

### 2. One-Time Pad with 27 symbols

The OTP demonstration uses this alphabet:

```text
abcdefghijklmnopqrstuvwxyz<space>
```

Each symbol is assigned an integer from 0 to 26. Encryption and decryption use modular arithmetic.

Encryption:

$$
C_i = (M_i + K_i) \bmod 27
$$

Decryption:

$$
M_i = (C_i - K_i + 27) \bmod 27
$$

Where:

- $$M_i$$ is the plaintext symbol at position \(i\).
- $$K_i$$ is the key symbol at position \(i\).
- $$C_i$$ is the ciphertext symbol at position \(i\).
- $$\bmod 27$$ keeps the result within the 27-symbol alphabet.

For the demonstration, the program derives a key from a selected plaintext and the fixed ciphertext. This is done to make the example reproducible and to show that different plaintexts can be compatible with the same ciphertext.

**Important:** a key derived from a known plaintext is not a secure way to create a real OTP key. In actual use, the key must be generated independently and uniformly at random.

## The 27-Character OTP Demonstration

The example uses the ciphertext:

```text
ANKYODKYUREPFJBYOJDSPLREYIUNOFDOIUERFPLUYTS
```

The program demonstrates two intended plaintexts:

```text
mr mustard with the candlestick in the hall
miss scarlet with the knife in the library
```

With a corresponding key for each plaintext, the same ciphertext can decrypt to either message.

This does **not** mean the program has discovered the original message. Rather, it constructs valid keys that make each chosen plaintext consistent with the ciphertext. Without additional information, the ciphertext alone does not tell an attacker which plaintext was intended.

For this 27-symbol variant, ciphertext and plaintext must use the same alphabet convention and have equal lengths. The exact strings and key material must be checked carefully when reproducing examples from textbooks, since a single missing character changes all subsequent positions.

## Why the One-Time Pad Is Unbreakable in Theory

A correctly used One-Time Pad has **perfect secrecy**: observing the ciphertext does not change an attacker's probability distribution over possible plaintexts, assuming the key is uniformly random, secret, and as long as the message.

For every plaintext \(M\) and ciphertext \(C\) of the same length, there is exactly one key \(K\) that maps \(M\) to \(C\). If all keys are equally likely, every candidate plaintext of the appropriate length is therefore equally compatible with the observed ciphertext.

In information-theoretic terms, perfect secrecy is expressed as:

$$
P(M \mid C) = P(M)
$$

Equivalently, the mutual information between the plaintext and ciphertext is zero:

$$
I(M;C) = 0
$$

These statements mean that the ciphertext provides no information about the plaintext, provided the OTP's assumptions hold.

### Why frequency analysis does not work against a proper OTP

A substitution cipher preserves patterns and the frequency distribution of letters, so statistical analysis can reveal clues about the original text.

A proper OTP does not preserve those useful statistical relationships. Each symbol is combined with an independent, uniformly random key symbol. The resulting ciphertext is statistically independent of the plaintext.

There is consequently no general ciphertext-only method that can identify the intended plaintext from a correctly generated OTP ciphertext alone. An attacker can guess candidate messages, but each candidate can be paired with a corresponding key. The ciphertext itself does not distinguish the correct candidate.

### “Unbreakable” does not mean “impossible to attack in every way”

Perfect secrecy is a mathematical property of the cipher under specific assumptions. It does not protect against every operational failure.

An OTP can fail if:

- The key is predictable or not truly random.
- The key is shorter than the message and repeated.
- The same key is reused for multiple messages.
- The key is stolen, copied, or disclosed.
- The plaintext is exposed before encryption or after decryption.
- An attacker alters the ciphertext and the receiver has no separate integrity protection.

Key distribution and storage are particularly difficult in practice: sender and receiver must securely possess the same amount of random key material as the message they want to exchange.

## Requirements for Perfect Secrecy

A secure One-Time Pad requires all of the following:

1. **Uniform randomness:** every possible key of the required length must be generated according to the intended uniform distribution.
2. **Message-length key:** the key must be at least as long as the plaintext, with one independent key symbol per plaintext symbol.
3. **Single use:** never reuse any portion of a key.
4. **Confidentiality:** keep the key secret from anyone who should not read the message.
5. **Secure key handling:** protect the key during generation, storage, distribution, and destruction.
6. **Consistent encoding:** sender and receiver must agree on the alphabet or byte encoding.
7. **Separate integrity protection:** OTP encryption alone does not authenticate a message or detect tampering.

## Build and Run

You need a C++17-compatible compiler, such as GCC or Clang.

Compile:

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o otp_lab
```

Run:

```bash
./otp_lab
```

The program menu provides options for the substitution-cipher experiment, the OTP demonstration, or both.

## Limitations and Security Notes

- This project is an educational demonstration, not a vetted cryptographic library.
- The substitution cipher's frequency-analysis routine is deliberately simple and is not expected to reliably recover every key.
- `std::mt19937` is a pseudorandom number generator and is **not suitable for generating cryptographic keys**.
- The OTP example derives keys from selected plaintexts and a fixed ciphertext to demonstrate the mathematics. These derived keys must not be used to protect real data.
- The 27-character implementation handles only English letters and spaces. It is not a general-purpose byte-oriented OTP implementation.
- Real applications usually use established authenticated-encryption algorithms, such as AES-GCM or ChaCha20-Poly1305, because securely generating and distributing message-length OTP keys is often impractical.

## References

- Claude E. Shannon, *Communication Theory of Secrecy Systems*, Bell System Technical Journal, 1949.
- NIST Computer Security Resource Center, Glossary: [One-Time Pad](https://csrc.nist.gov/glossary/term/one_time_pad).
- Encyclopaedia Britannica, background on cryptography and historical cipher systems.

---

**Disclaimer:** This repository is intended for educational purposes. Do not use the demonstration code to protect sensitive information.
