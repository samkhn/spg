#ifndef CRYPT_SPG_ENCRYPT
#define CRYPT_SPG_ENCRYPT

struct Status {
  int return_code;
};

enum class EncryptMode { Encrypt, Decrypt, Unknown };

Status Encryptor(EncryptMode mode);

#endif  // CRYPT_SPG_ENCRYPT
