#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"

rm -rf "$BUILD_DIR"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DBUILD_TESTING=ON -DCMAKE_CXX_STANDARD=${CXX_STANDARD:-17}
cmake --build "$BUILD_DIR" --config Release
cd "$BUILD_DIR"
ctest --output-on-failure
