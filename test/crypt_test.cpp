#include "gtest/gtest.h"
#include "testing/fakecrypt.hpp"

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::FakeCrypt;

TEST(EncryptTest, EncryptAMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  EXPECT_TRUE(crypt_module);
  auto result = crypt_module->Encrypt("2", "1");
  auto ciphertext = result.value_or("nil");
  EXPECT_EQ("12", ciphertext);
}

TEST(EncryptTest, EncryptThenDecryptAMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  std::string key = "4";
  std::string message = "3";
  auto result = crypt_module->Encrypt(key, message);
  auto ciphertext = result.value_or("nil");
  ASSERT_EQ("34", ciphertext);
  result = crypt_module->Decrypt(key, ciphertext);
  auto plaintext = result.value_or("nil");
  EXPECT_EQ(message, plaintext);
}

TEST(EncryptTest, ReturnsNothingWithUnrelatedKey) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  std::string key = "4";
  std::string unrelated_key = "1";
  std::string message = "3";
  auto result = crypt_module->Encrypt(key, message);
  auto ciphertext = result.value_or("nil");
  ASSERT_EQ("34", ciphertext);
  // ciphertext used key "4". we are using key "1".
  result = crypt_module->Decrypt(unrelated_key, ciphertext);
  auto plaintext = result.value_or("nil");
  EXPECT_EQ("nil", plaintext);
}

// TODO(testing/fake): The problem here with the fake encoding is that if we use
// key A and message B and then modify message B within the key, then the Crypt
// library still decrypts it. We should add error codes to the fakecrypt.
TEST(EncryptTest, ReturnsNothingWithCorruptedMessage) {
  std::unique_ptr<Crypt> crypt_module = FakeCrypt::ConstructFakeCrypt();
  ASSERT_TRUE(crypt_module);
  std::string key = "4";
  std::string message = "3";
  auto result = crypt_module->Encrypt(key, message);
  auto ciphertext = result.value_or("nil");
  ASSERT_EQ("34", ciphertext);
  ciphertext = "corrupt";
  result = crypt_module->Decrypt(key, ciphertext);
  auto plaintext = result.value_or("nil");
  EXPECT_EQ("nil", plaintext);
}
