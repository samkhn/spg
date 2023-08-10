#ifndef SPG_CRYPT_TINKCRYPT
#define SPG_CRYPT_TINKCRYPT

#include <memory>

#include "crypt.hpp"

namespace SPG {
namespace Crypt {

// TinkCrypt is a wrapper around the Tink Cryptography Library
class TinkCrypt : public Crypt {
 public:
  static std::unique_ptr<TinkCrypt> ConstructCrypt();
  std::optional<std::string> Encrypt(std::string_view key,
                                     std::string_view plaintext) override;
  std::optional<std::string> Decrypt(std::string_view key,
                                     std::string_view ciphertext) override;

 private:
  TinkCrypt() = default;
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_TINKCRYPT
