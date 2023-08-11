#include "sodiumcrypt.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::SodiumCrypt;
using ::testing::ContainerEq;
using ::testing::ElementsAre;

TEST(SodiumCryptTest, EncryptAndDecryptAMessage) {
  std::unique_ptr<Crypt> crypt_module = SodiumCrypt::ConstructCrypt();
  EXPECT_TRUE(crypt_module);
  ByteVector key = {'2'};
  ByteVector message = {'1'};
  auto encrypt_result = crypt_module->Encrypt(key, message);
  ASSERT_TRUE(encrypt_result.has_value());
  EXPECT_NE(encrypt_result.value().size(), 0);
  auto decrypt_result = crypt_module->Decrypt(key, encrypt_result.value());
  ASSERT_TRUE(decrypt_result.has_value());
  EXPECT_THAT(decrypt_result.value(), ContainerEq(message));
}
