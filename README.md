# spg

Samiur's privacy generator.

## Requirements

Tools:

* CMake
* C++ compiler with at least C++17 support
* Linux (coming soon for Windows)

Libraries:

* libsodium

## Build

```bash
$ mkdir build
$ cmake -B build -S .
```

## Use

```bash
$ spgencrypt -k {key} -m {message}
$ spgdecrypt -k {key}
```
Note:

* these arguments are required
* uses ChaCha20 under the hood so: key must be representable within 32 bytes, message cannot exceed 256 GiB

## Contributing

* Google Test
* Python3.7
* Formatters: clang-format and yapf

### Testing

```bash
$ cmake --build $BUILD_DIR --target check-all  # runs unit tests and integration tests
$ # or if you just want to run integration tests: cmake --build $BUILD_DIR --target integration_test
```

### Future plans

* nonce support
* Internal key management
* Multiple messages decrypt support
* Support for more cryptographic primitives (just libsodium at the moment).
* Interactive mode (so that we can hide plaintext message from getting typed into user terminal).
* Windows support (we have compiler flags at the moment but need additional context cracking).
