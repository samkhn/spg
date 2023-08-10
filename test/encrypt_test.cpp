#include "encrypt.hpp"

#include "gtest/gtest.h"

TEST(EncryptTest, ReturnsZeroForNow) {
  EXPECT_TRUE(Encryptor(EncryptMode::Encrypt).return_code == 0);
}
