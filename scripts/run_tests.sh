#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"

rm -rf "$BUILD_DIR"
cmake_args=(-S "$ROOT_DIR" -B "$BUILD_DIR" -DBUILD_TESTING=ON -DCMAKE_CXX_STANDARD=${CXX_STANDARD:-17})
if [[ -n "${VCPKG_ROOT:-}" ]]; then
  cmake_args+=(-DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
fi
cmake "${cmake_args[@]}"
cmake --build "$BUILD_DIR" --config Release
cd "$BUILD_DIR"
ctest --output-on-failure
