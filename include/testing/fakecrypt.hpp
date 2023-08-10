#ifndef SPG_CRYPT_TESTING_FAKECRYPT
#define SPG_CRYPT_TESTING_FAKECRYPT

#include <memory>

#include "crypt.hpp"

namespace SPG {
namespace Crypt {

// FakeCrypt returns fake cryptographic or plaintext data
// Used to test Crypt wrapper library for underlying cryptographic primitives.
class FakeCrypt : public Crypt {
 public:
  static std::unique_ptr<FakeCrypt> ConstructFakeCrypt();
  std::optional<std::string> Encrypt(std::string_view key,
                                     std::string_view plaintext) override;
  std::optional<std::string> Decrypt(std::string_view key,
                                     std::string_view ciphertext) override;

 private:
  FakeCrypt() = default;
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_TESTING_FAKECRYPT
