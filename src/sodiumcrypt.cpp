// TODO(correctness): we are zeroing the nonce. Is that okay?

#include "sodiumcrypt.hpp"

#include <sodium.h>

using ::SPG::Crypt::SodiumCrypt;

std::unique_ptr<SodiumCrypt> SodiumCrypt::ConstructCrypt() {
  if (sodium_init() == -1) {
    return {};
  }
  return std::unique_ptr<SodiumCrypt>(new SodiumCrypt());
};

std::optional<ByteVector> SodiumCrypt::Encrypt(const ByteVector& key,
                                               const ByteVector& plaintext) {
  unsigned long long ciphertext_length;
  ByteVector ciphertext;
  ciphertext.resize(plaintext.size() +
                    crypto_aead_chacha20poly1305_IETF_ABYTES);
  ByteVector nonce(crypto_aead_chacha20poly1305_IETF_NPUBBYTES, '0');
  ByteVector processed_key(crypto_aead_chacha20poly1305_IETF_KEYBYTES, '0');
  processed_key = key;
  if (crypto_aead_chacha20poly1305_ietf_encrypt(
          ciphertext.data(), &ciphertext_length, plaintext.data(),
          plaintext.size(), NULL, 0, NULL, nonce.data(),
          processed_key.data()) == 0) {
    return ByteVector(ciphertext.begin(),
                      ciphertext.begin() + ciphertext_length);
  }
  return std::nullopt;
}

std::optional<ByteVector> SodiumCrypt::Decrypt(const ByteVector& key,
                                               const ByteVector& ciphertext) {
  unsigned long long plaintext_length;
  ByteVector plaintext;
  plaintext.resize(ciphertext.size() +
                   crypto_aead_chacha20poly1305_IETF_ABYTES);
  ByteVector nonce(crypto_aead_chacha20poly1305_IETF_NPUBBYTES, '0');
  ByteVector processed_key(crypto_aead_chacha20poly1305_IETF_KEYBYTES, '0');
  processed_key = key;
  if (crypto_aead_chacha20poly1305_ietf_decrypt(
          plaintext.data(), &plaintext_length, NULL, ciphertext.data(),
          ciphertext.size(), NULL, 0, nonce.data(),
          processed_key.data()) == 0) {
    return ByteVector(plaintext.begin(), plaintext.begin() + plaintext_length);
  }
  return std::nullopt;
}
