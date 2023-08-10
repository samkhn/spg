#include "sodiumcrypt.hpp"

using ::SPG::Crypt::SodiumCrypt;

std::unique_ptr<SodiumCrypt> SodiumCrypt::ConstructCrypt() {
  return std::unique_ptr<SodiumCrypt>(new SodiumCrypt());
};

std::optional<std::string> SodiumCrypt::Encrypt(std::string_view key,
                                                std::string_view plaintext) {}

std::optional<std::string> SodiumCrypt::Decrypt(std::string_view key,
                                                std::string_view ciphertext) {}
