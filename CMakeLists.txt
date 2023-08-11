# TODO(refactor): Lots of instances of
# target_include_dir(* PUBLIC include PRIVATE src). Replace with macro.
cmake_minimum_required(VERSION 3.6)

project(spg
  DESCRIPTION "Samiur's Privacy Generator"
  VERSION 0.0.1
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Tooling and Dependencies: Google Test and libsodium
# Google Test will be downloaded when you initially configure this project
# if it's not available on your machine.
# libsodium is expected to be installed on your machine (no download)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

enable_testing()
find_package(GTest REQUIRED)

if(NOT GTest_FOUND)
  FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
    DOWNLOAD_EXTRACT_TIMESTAMP true
  )
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

find_package(Sodium REQUIRED)

# Main project targets
# TODO(refactor): We are duplicating the compile_options flags. Let's move into
# a variable.

# byte_vector aka a contiguous container of unsigned chars
# TODO(correctness): byte_vector_tests?
add_library(byte_vector INTERFACE)
target_include_directories(byte_vector INTERFACE include)

# crypt - interface for crypt wrappers to implement
add_library(crypt INTERFACE)
target_include_directories(crypt INTERFACE include)

# crypt_test - unit test for crypt interface library
add_executable(crypt_test test/crypt_test.cpp)
target_include_directories(crypt_test
  PUBLIC include
  PRIVATE src
)
target_link_libraries(crypt_test GTest::gtest_main fakecrypt)

# fakecrypt - used in unit tests of crypt implementors
add_library(fakecrypt
  STATIC src/testing/fakecrypt.cpp include/testing/fakecrypt.hpp
)
if (MSVC)
  target_compile_options(fakecrypt
    PUBLIC /W4 /w14640
  )
else()
  target_compile_options(fakecrypt
    PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
  )
endif()
target_include_directories(fakecrypt
  PUBLIC include
  PRIVATE src
)
target_link_libraries(fakecrypt crypt)

# sodiumcrypt - wrapper around libsodium
add_library(sodiumcrypt
  STATIC src/sodiumcrypt.cpp include/sodiumcrypt.hpp
)
if (MSVC)
  target_compile_options(sodiumcrypt
    PUBLIC /W4 /w14640
  )
else()
  target_compile_options(sodiumcrypt
    PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
  )
endif()
target_include_directories(sodiumcrypt
  PUBLIC include sodium_INCLUDE_DIR
  PRIVATE src
)
target_link_libraries(sodiumcrypt sodium crypt)

# sodiumcrypt_test - unit test for the sodium wrapper library
add_executable(sodiumcrypt_test test/sodiumcrypt_test.cpp)
target_include_directories(sodiumcrypt_test
  PUBLIC include
  PRIVATE src
)
target_link_libraries(sodiumcrypt_test GTest::gtest_main sodiumcrypt)

# spgencrypt - user facing binary to encrypt a message with a key
add_executable(spgencrypt src/spgencrypt.cpp)
target_include_directories(spgencrypt
  PUBLIC include
  PRIVATE src
)
target_link_libraries(spgencrypt sodiumcrypt)

# spgdecrypt - user facing binary to decrypt a message with a key
add_executable(spgdecrypt src/spgdecrypt.cpp)
target_include_directories(spgdecrypt
  PUBLIC include
  PRIVATE src
)
target_link_libraries(spgdecrypt sodiumcrypt)

# spg_integration_test tests the command line tools
add_custom_target(integration_test
  python3 ${CMAKE_SOURCE_DIR}/test/integration.py $<TARGET_FILE:spgencrypt> $<TARGET_FILE:spgdecrypt>
  DEPENDS spgencrypt
  DEPENDS spgdecrypt
)

# REACH TODOs
# TODO: context library (for determining if Win or Linux)
#  Used to cli library and binaries to determine where "HOME" is.
# TODO: flags library (used by command line)
# TODO: context/cli library (used by interactive mode)

# All target. So you can run: cmake --build $BUILD_DIR --target check-all
add_custom_target(check-all
  DEPENDS crypt_test
  DEPENDS sodiumcrypt_test
  COMMAND $<TARGET_FILE:crypt_test>
  COMMAND $<TARGET_FILE:sodiumcrypt_test>
  DEPENDS integration_test
)
