#ifndef SPG_CRYPT_LIB_BYTEVECTOR
#define SPG_CRYPT_LIB_BYTEVECTOR

#include <iostream>
#include <string_view>
#include <vector>

namespace SPG {
namespace Crypt {
namespace Lib {

// TODO(optimization): benchmark this against std::array
using Byte = unsigned char;
using ByteVector = std::vector<Byte>;

inline ByteVector FromString(std::string_view s) {
  ByteVector bv;
  for (auto c : s) {
    bv.push_back(static_cast<unsigned char>(c));
  }
  return bv;
}

};  // namespace Lib
};  // namespace Crypt
};  // namespace SPG

inline std::istream& operator>>(std::istream& in,
                                ::SPG::Crypt::Lib::ByteVector& bv) {
  ::SPG::Crypt::Lib::Byte b;
  while (in >> b) {
    bv.push_back(b);
  }
  return in;
}

inline std::ostream& operator<<(std::ostream& out,
                                const ::SPG::Crypt::Lib::ByteVector& bv) {
  for (auto c : bv) {
    out << c;
  }
  return out;
}

#endif  // SPG_CRYPT_LIB_BYTEVECTOR
