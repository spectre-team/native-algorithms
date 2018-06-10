FROM ubuntu:trusty

ARG GCC_VERSION=7

## Deps phase ##

RUN echo "deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu trusty main" | tee /etc/apt/sources.list.d/ubuntu-toolchain.list \
 && echo "deb http://ppa.launchpad.net/lkoppel/opencv/ubuntu trusty main" | tee /etc/apt/sources.list.d/opencv.list \
 && apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 98749F984560FB8A 1E9377A2BA9EF27F \
 && apt-get -qq update \
 && apt-get install -qq \
    build-essential \
    g++-${GCC_VERSION} \
    gdb \
    git \
    libstdc++-7-dev \
    libopencv-dev \
    libiomp-dev \
    strace \
    wget \
 && wget https://cmake.org/files/v3.11/cmake-3.11.2-Linux-x86_64.sh \
 && sh cmake-3.11.2-Linux-x86_64.sh --prefix=/usr/local --exclude-subdir

## Build phase ##

WORKDIR /data
COPY . /data

RUN mkdir build \
 && cd build \
 && export CC="/usr/bin/gcc-${GCC_VERSION}" \
 && export CXX="/usr/bin/g++-${GCC_VERSION}" \
 && cmake .. \
 && cmake --build . -- -j$(nproc) \
 && ctest --output-on-failure -j$(nproc)
