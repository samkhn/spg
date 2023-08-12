#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>

#include "sodiumcrypt.hpp"

using ::SPG::Crypt::Crypt;
using ::SPG::Crypt::SodiumCrypt;
using ::SPG::Crypt::Lib::ByteVector;
using ::SPG::Crypt::Lib::FromString;

static constexpr std::string_view kUsage = "Usage: spgdecrypt -k {key}\n";
static constexpr std::string_view kKeyFlag = "-k";

int main(int argc, char** argv) {
  std::string key_string;
  for (int i = 1; i < argc; ++i) {
    if (kKeyFlag.compare(argv[i]) == 0) {
      if (i + 1 < argc) {
        key_string = argv[++i];
      }
    } else {
      std::cout << kUsage << '\n';
      return -2;
    }
  }
  if (key_string.empty()) {
    std::cout << "Missing decryption key\n";
    return -2;
  }
  ByteVector key = FromString(key_string);
  const std::regex extension_matcher{R"(.*\.spg)"};
  std::unique_ptr<Crypt> crypt_module = SodiumCrypt::ConstructCrypt();
  int decrypted_message_count = 0;
  // TODO(platform): Add windows specific instructions
  // pass NULL as the module handle to GetModuleFileName.
  std::vector<std::filesystem::path> spg_files;
  std::filesystem::path currentdir =
      std::filesystem::canonical("/proc/self/exe").parent_path();
  for (const auto& dir_entry :
       std::filesystem::directory_iterator{currentdir}) {
    if (dir_entry.is_regular_file() &&
        std::regex_match(dir_entry.path().string(), extension_matcher)) {
      std::filesystem::file_time_type ftime =
          std::filesystem::last_write_time(dir_entry);
      unsigned long long time_since_last_write =
          static_cast<unsigned long long>(ftime.time_since_epoch().count());
      std::cout << "spg file found: " << dir_entry.path()
                << ". Last written to: " << time_since_last_write << " \n";
      spg_files.push_back(dir_entry);
    }
  }
  for (const auto& spg_file : spg_files) {
    std::ifstream cipherfile;
    cipherfile.open(spg_file);
    ByteVector message;
    cipherfile >> message;
    auto decrypted_message = crypt_module->Decrypt(key, message);
    if (decrypted_message.has_value()) {
      std::cout << decrypted_message.value() << '\n';
      decrypted_message_count += 1;
    }
    cipherfile.close();
  }
  if (decrypted_message_count == 0) {
    std::cout << "Failed to decrypt\n";
    return -1;
  }
  std::cout << "Decrypted " << decrypted_message_count << " ciphers\n";
  return 0;
}
