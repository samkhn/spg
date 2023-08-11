#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testing/fakecrypt.hpp"

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::FakeCrypt;
using ::testing::ContainerEq;
using ::testing::ElementsAre;

TEST(CryptInterfaceTest, EncryptAMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  EXPECT_TRUE(crypt_module);
  auto result = crypt_module->Encrypt({'2'}, {'1'});
  auto ciphertext = result.value_or(ByteVector());
  EXPECT_THAT(ciphertext, ElementsAre('1', '2'));
}

TEST(CryptInterfaceTest, EncryptThenDecryptAMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  ByteVector key = {'4'};
  ByteVector message = {'3'};
  auto encrypt_result = crypt_module->Encrypt(key, message);
  ASSERT_TRUE(encrypt_result.has_value());
  ASSERT_THAT(encrypt_result.value(), ElementsAre('3', '4'));
  auto decrypt_result = crypt_module->Decrypt(key, encrypt_result.value());
  ASSERT_TRUE(decrypt_result.has_value());
  EXPECT_THAT(decrypt_result.value(), ContainerEq(message));
}

TEST(CryptInterfaceTest, ReturnsNothingWithUnrelatedKey) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  ByteVector key = {'4'};
  ByteVector message = {'3'};
  ByteVector unrelated_key = {'1'};
  auto result = crypt_module->Encrypt(key, message);
  ASSERT_TRUE(result.has_value());
  ASSERT_THAT(result.value(), ElementsAre('3', '4'));
  // ciphertext used key "4". we are using key "1".
  EXPECT_EQ(std::nullopt, crypt_module->Decrypt(unrelated_key, result.value()));
}

// TODO(testing/fake): The problem here with the fake encoding is that if we use
// key A and message B and then modify message B within the key, then the Crypt
// library still decrypts it. We should add error codes to the fakecrypt.
TEST(CryptInterfaceTest, ReturnsNothingWithCorruptedMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  ByteVector key = {'4'};
  ByteVector message = {'3'};
  auto result = crypt_module->Encrypt(key, message);
  ASSERT_TRUE(result.has_value());
  ASSERT_THAT(result.value(), ElementsAre('3', '4'));
  ByteVector ciphertext = {'c', 't'};
  EXPECT_EQ(std::nullopt, crypt_module->Decrypt(key, ciphertext));
}
