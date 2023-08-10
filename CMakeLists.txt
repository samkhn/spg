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

# spgencrypt is a wrapper library around Google Tink (in case it comes out that Tink is insecure or doesn't pass client testing/requirements, we can swap it).
add_library(spgencrypt STATIC src/encrypt.cpp include/encrypt.hpp)
if (MSVC)
  target_compile_options(spgencrypt PUBLIC "/W4 /w14640")
else()
  target_compile_options(spgencrypt PUBLIC "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
endif()

target_include_directories(spgencrypt PUBLIC include PRIVATE src)
target_link_libraries(spgencrypt tink::static)

add_executable(encrypt_test test/encrypt_test.cpp)
target_link_libraries(encrypt_test GTest::gtest_main spgencrypt)

# All target. So you can run: cmake --build $BUILD_DIR --target check-all
add_custom_target(check-all
  DEPENDS encrypt_test COMMAND $<TARGET_FILE:encrypt_test>
)
