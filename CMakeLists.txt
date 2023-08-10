cmake_minimum_required(VERSION 3.6)

project(spg
  DESCRIPTION "Samiur's Privacy Generator"
  VERSION 0.0.1
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Dependencies: Google Test and Google Tink

FetchContent_Declare(tink
  URL       https://github.com/tink-crypto/tink-cc/archive/refs/tags/v2.0.0.zip
  URL_HASH  SHA256=103ddfce800e77f3b3b6b2c808a8611bc734b31ddb12fbcfd8bebc1b96a7e963
)
# Google Tink transitively pulls in Google Abseil, protobuf, BoringSSL, rapidjson
set(ABSL_PROPAGATE_CXX_STD ON)

FetchContent_GetProperties(tink)
if(NOT googletest_POPULATED)
  FetchContent_Populate(tink)
  add_subdirectory(${tink_SOURCE_DIR} ${tink_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# Main project targets
# TODO(refactor): We are duplicating the compile_options flags. Let's move into a variable.

# crypt - interface for crypt wrappers to implement
add_library(crypt INTERFACE)
target_include_directories(crypt INTERFACE include)

# fakecrypt - used in unit tests of crypt implementors
add_library(fakecrypt STATIC src/testing/fakecrypt.cpp include/testing/fakecrypt.hpp)
if (MSVC)
  target_compile_options(fakecrypt PUBLIC "/W4 /w14640")
else()
  target_compile_options(fakecrypt PUBLIC "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
endif()
target_include_directories(fakecrypt PUBLIC include PRIVATE src)
target_link_libraries(fakecrypt crypt)

# tinkcrypt - wrapper around Google Tink
add_library(tinkcrypt STATIC src/tinkcrypt.cpp include/tinkcrypt.hpp)
if (MSVC)
  target_compile_options(tinkcrypt PUBLIC "/W4 /w14640")
else()
  target_compile_options(tinkcrypt PUBLIC "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
endif()
target_include_directories(tinkcrypt PUBLIC include PRIVATE src)
target_link_libraries(tinkcrypt crypt tink::static)

# crypt_test - unit test for crypt library
add_executable(crypt_test test/crypt_test.cpp)
target_include_directories(crypt_test PUBLIC include PRIVATE src)
target_link_libraries(crypt_test GTest::gtest_main fakecrypt)

# TODO: sgencrypt binary
# TODO: sgencrypt_it script
# TODO: sgdecrypt binary
# TODO: sgdecrypt_it script

# REACH TODOs
# TODO: context library (for determining if Win or Linux)
#  Used to cli library and binaries to determine where "HOME" is.
# TODO: flags library (used by command line)
# TODO: context/cli library (used by interactive mode)

# All target. So you can run: cmake --build $BUILD_DIR --target check-all
add_custom_target(check-all
  DEPENDS encrypt_test COMMAND $<TARGET_FILE:crypt_test>
)
