FROM ubuntu:latest

WORKDIR /app

RUN apt-get update && apt-get install -y cmake mingw-w64 git curl zip unzip tar

RUN git clone https://github.com/microsoft/vcpkg.git && \
    cd vcpkg && \
    ./bootstrap-vcpkg.sh

COPY . .

ENTRYPOINT cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=/usr/bin/x86_64-w64-mingw32-gcc \
    -DCMAKE_EXE_LINKER_FLAGS="-static" \
    -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-x86_64-w64-mingw32.cmake && \
    cmake --build build