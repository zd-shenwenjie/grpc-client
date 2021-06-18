# GRPC Client (C++)

using c++ + grpc

#### Getting started
- Clone the repository
```
git@github.com:zd-shenwenjie/grpc-client.git
cd grpc-client
```
- Init submodule (proto)
```
git submodule init
```
- Update submodule
```
git submodule update
```
#### Building with CMake
```
mkdir -p cmake/build
pushd "cmake/build"
cmake ../..
make
popd
```
#### Build with CMake for ARM
```
mkdir -p "cmake/build_arm"
pushd "cmake/build_arm"
cmake -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DProtobuf_DIR=/tmp/stage/lib/cmake/protobuf \
      -DgRPC_DIR=/tmp/stage/lib/cmake/grpc \
      ../..
make
popd
```

#### How to run
```
 ./grpc-client -consumer
 ./grpc-client -observer
 ./grpc-client -provider
```
