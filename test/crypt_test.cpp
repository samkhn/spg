#include "gtest/gtest.h"
#include "testing/fakecrypt.hpp"

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::FakeCrypt;

TEST(EncryptTest, DefaultConstructPlaceholder) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  EXPECT_TRUE(crypt_module);
}
