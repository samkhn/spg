#include <fstream>
#include <iostream>
#include <random>
#include <string_view>

#include "sodiumcrypt.hpp"

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::SodiumCrypt;
using ::SPG::Crypt::Lib::ByteVector;
using ::SPG::Crypt::Lib::FromString;

static constexpr std::string_view kUsage =
    "Usage: spgencrypt -k {key} -m {message}\n";
static constexpr std::string_view kFileExtension = ".spg";
static constexpr std::string_view kKeyFlag = "-k";
static constexpr std::string_view kMessageFlag = "-m";

int main(int argc, char **argv) {
  std::string key_string, message_string;
  for (int i = 1; i < argc; ++i) {
    if (kKeyFlag.compare(argv[i]) == 0) {
      if (i + 1 < argc) {
        key_string = argv[++i];
      } else {
        std::cout << kUsage << '\n';
        return -2;
      }
    } else if (kMessageFlag.compare(argv[i]) == 0) {
      if (i + 1 < argc) {
        message_string = argv[++i];
      } else {
        std::cout << kUsage << '\n';
        return -2;
      }
    } else {
      std::cout << kUsage << '\n';
      return -2;
    }
  }
  if (key_string.empty()) {
    std::cout << "Missing encryption key\n";
    return -2;
  }
  if (message_string.empty()) {
    std::cout << "Missing message to encrypt\n";
    return -2;
  }
  ByteVector key = FromString(key_string);
  ByteVector message = FromString(message_string);
  std::unique_ptr<Crypt> crypt_module = SodiumCrypt::ConstructCrypt();
  auto encrypted_message = crypt_module->Encrypt(key, message);
  if (!encrypted_message.has_value()) {
    std::cout << "Failed to encrypt\n";
    return -1;
  }
  std::random_device rd;
  std::mt19937 generator(rd());
  std::string filename = std::to_string(generator());
  filename += kFileExtension;
  std::ofstream f;
  f.open(filename, std::ios::binary);
  f << encrypted_message.value();
  f.close();
  std::cout << "Encryption succeeded. Generated " << filename << '\n';
  return 0;
}
