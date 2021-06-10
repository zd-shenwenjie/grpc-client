gRPC C++ - Building from source

# Pre-requisites（Linux）：
apt-get install build-essential autoconf libtool pkg-config
apt-get install gcc-aarch64-linux-gnu
apt-get install g++-aarch64-linux-gnu
apt-get update && apt-get install -y libssl-dev

# Install CMake 3.16 (CMake v3.13 or newer)
apt-get update && apt-get install -y wget
wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.16.1/cmake-3.16.1-Linux-x86_64.sh
sh cmake-linux.sh -- --skip-license --prefix=/usr
rm cmake-linux.sh

# Clone the repository (including submodules)
git clone --recurse-submodules -b v1.38.0 https://github.com/grpc/grpc
cd grpc
git submodule update --init

# Building with CMake
mkdir -p cmake/build
pushd "cmake/build"
cmake ../..
make
popd

# Install absl
mkdir -p "third_party/abseil-cpp/cmake/build"
pushd "third_party/abseil-cpp/cmake/build"
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../..
make -j4 install
popd

# Install after build
mkdir -p "cmake/build"
pushd "cmake/build"
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DgRPC_INSTALL=ON \
  -DgRPC_BUILD_TESTS=OFF \
  -DgRPC_ABSL_PROVIDER=package \
  -DgRPC_SSL_PROVIDER=package \
  ../..
make -j4 install
popd

# Build helloworld example using cmake
mkdir -p "examples/cpp/helloworld/cmake/build"
pushd "examples/cpp/helloworld/cmake/build"
cmake ../..
make
popd

# ********************************************** Cross-compiling *********************************************************************************

# Write a toolchain file to use for cross-compiling.
cat > /tmp/toolchain.cmake <<'EOT'
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_STAGING_PREFIX /tmp/stage)
set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc-9) 
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++-9) 
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
EOT

# Build and install absl (absl won't be installed down below)
mkdir -p "third_party/abseil-cpp/cmake/build_arm"
pushd "third_party/abseil-cpp/cmake/build_arm"
cmake -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/tmp/install \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
      ../..
make -j4 install
popd

# Build and install gRPC for ARM. This build will use the host architecture copies of protoc and grpc_cpp_plugin that we built earlier because we installed them to a location in our PATH (/usr/local/bin).
mkdir -p "cmake/build_arm"
pushd "cmake/build_arm"
cmake -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/tmp/install \
      ../..
make -j4 install
popd

# Build helloworld example for ARM. As above, it will find and use protoc and grpc_cpp_plugin for the host architecture.
mkdir -p "examples/cpp/helloworld/cmake/build_arm"
pushd "examples/cpp/helloworld/cmake/build_arm"
cmake -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DProtobuf_DIR=/tmp/stage/lib/cmake/protobuf \
      -DgRPC_DIR=/tmp/stage/lib/cmake/grpc \
      ../..
make
popd


参考：
https://github.com/grpc/grpc/blob/master/BUILDING.md#build-from-source
https://github.com/grpc/grpc/blob/master/test/distrib/cpp/run_distrib_test_cmake_aarch64_cross.sh
