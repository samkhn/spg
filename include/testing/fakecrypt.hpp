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
  std::optional<ByteVector> Encrypt(const ByteVector& key,
                                    const ByteVector& plaintext) override;
  std::optional<ByteVector> Decrypt(const ByteVector& key,
                                    const ByteVector& ciphertext) override;

 private:
  FakeCrypt() = default;
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_TESTING_FAKECRYPT
