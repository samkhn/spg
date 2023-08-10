#include "testing/fakecrypt.hpp"

using ::SPG::Crypt::FakeCrypt;

std::unique_ptr<FakeCrypt> FakeCrypt::ConstructFakeCrypt() {
  return std::unique_ptr<FakeCrypt>(new FakeCrypt());
}

std::optional<std::string> FakeCrypt::Encrypt(std::string_view key,
                                              std::string_view plaintext) {
  return std::string(plaintext) + std::string(key);
}

std::optional<std::string> FakeCrypt::Decrypt(std::string_view key,
                                              std::string_view ciphertext) {
  std::string result(ciphertext);
  auto key_pos = result.find(key);
  if (key_pos != std::string::npos) {
    result.erase(key_pos, key.size());
    return result;
  }
  return std::nullopt;
}
