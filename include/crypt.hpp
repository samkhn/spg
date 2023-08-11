#ifndef SPG_CRYPT_CRYPT
#define SPG_CRYPT_CRYPT

#include <optional>
#include <vector>

#include "lib/byte_vector.hpp"

namespace {

using ::SPG::Crypt::Lib::ByteVector;

};  // namespace

namespace SPG {
namespace Crypt {

// Pure virtual class. Cryptographic libraries must be wrapped in a class that
// implements this interface. Underlying method will generate heap allocated
// data.
class Crypt {
 public:
  virtual std::optional<ByteVector> Encrypt(const ByteVector& key,
                                            const ByteVector& plaintext) = 0;
  virtual std::optional<ByteVector> Decrypt(const ByteVector& key,
                                            const ByteVector& ciphertext) = 0;
  // Empty implementation so that all derived implementations have destructors
  // (especially inside unique_ptrs).
  virtual ~Crypt(){};
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_CRYPT
