#!/bin/bash
#set -x

if [ -x "$(command -v cindex)" ]; then
	cindex .
fi

PROJECT_DIR="$(echo "$(pwd)" | sed -e 's/^\/home\/samiurkhan\///')"
SOURCE_DIR=$(pwd)
BUILD_DIR=$HOME/build/$PROJECT_DIR

if [ ! -d "$BUILD_DIR" ]; then
	mkdir -p $BUILD_DIR
	cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
	      -B $BUILD_DIR -G Ninja \
	      -S $SOURCE_DIR
fi

cmake --build $BUILD_DIR --target check-all
