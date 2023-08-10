#include "tinkcrypt.hpp"

using ::SPG::Crypt::TinkCrypt;

static std::unique_ptr<TinkCrypt> TinkCrypt::ConstructCrypt() {
  return std::unique_ptr<TinkCrypt>(new TinkCrypt());
};

std::optional<std::string> TinkCrypt::Encrypt(std::string_view key,
                                              std::string_view plaintext) {}

std::optional<std::string> TinkCrypt::Decrypt(std::string_view key,
                                              std::string_view ciphertext) {}
