#!/bin/bash
cmake . -B build/ -G "Unix Makefiles" -DBUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=$1