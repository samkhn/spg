#ifndef SPG_CRYPT_CRYPT
#define SPG_CRYPT_CRYPT

#include <optional>
#include <string>
#include <string_view>

namespace SPG {
namespace Crypt {

// Pure virtual class. Cryptographic libraries must be wrapped in a class that
// implements this interface. Underlying method will generate heap allocated
// data.
class Crypt {
 public:
  virtual std::optional<std::string> Encrypt(std::string_view key,
                                             std::string_view plaintext) = 0;
  virtual std::optional<std::string> Decrypt(std::string_view key,
                                             std::string_view ciphertext) = 0;
  // Empty implementation so that all derived implementations have destructors
  // (especially inside unique_ptrs).
  virtual ~Crypt(){};
};

};  // namespace Crypt
};  // namespace SPG

#endif  // SPG_CRYPT_CRYPT
