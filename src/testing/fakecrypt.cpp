#include "testing/fakecrypt.hpp"

#include <algorithm>

using ::SPG::Crypt::FakeCrypt;

std::unique_ptr<FakeCrypt> FakeCrypt::ConstructFakeCrypt() {
  return std::unique_ptr<FakeCrypt>(new FakeCrypt());
}

std::optional<ByteVector> FakeCrypt::Encrypt(const ByteVector& key,
                                             const ByteVector& plaintext) {
  ByteVector result(plaintext);
  result.insert(result.end(), key.begin(), key.end());
  return result;
}

std::optional<ByteVector> FakeCrypt::Decrypt(const ByteVector& key,
                                             const ByteVector& ciphertext) {
  ByteVector result(ciphertext);
  auto it =
      std::search(ciphertext.begin(), ciphertext.end(), key.begin(), key.end());
  if (it != ciphertext.end()) {
    result.erase(it, it + key.size());
    return result;
  }
  return std::nullopt;
}
