#ifndef SPG_CRYPT_SODIUMCRYPT
#define SPG_CRYPT_SODIUMCRYPT

#include <memory>
#include <sodium.h>

#include "crypt.hpp"

namespace SPG {
namespace Crypt {

// SodiumCrypt is a wrapper around the Tink Cryptography Library
class SodiumCrypt : public Crypt {
 public:
  static std::unique_ptr<SodiumCrypt> ConstructCrypt();
  std::optional<std::string> Encrypt(std::string_view key,
                                     std::string_view plaintext) override;
  std::optional<std::string> Decrypt(std::string_view key,
                                     std::string_view ciphertext) override;

 private:
  SodiumCrypt() = default;
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_SODIUMCRYPT
