#ifndef SPG_CRYPT_SODIUMCRYPT
#define SPG_CRYPT_SODIUMCRYPT

#include <memory>

#include "crypt.hpp"

namespace SPG {
namespace Crypt {

// SodiumCrypt is a wrapper around the Tink Cryptography Library
class SodiumCrypt : public Crypt {
 public:
  static std::unique_ptr<SodiumCrypt> ConstructCrypt();
  std::optional<ByteVector> Encrypt(const ByteVector& key,
                                    const ByteVector& plaintext) override;
  std::optional<ByteVector> Decrypt(const ByteVector& key,
                                    const ByteVector& ciphertext) override;

 private:
  SodiumCrypt() = default;
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_SODIUMCRYPT
