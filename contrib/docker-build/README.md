# Docker-based build images

Meant for local testing, before sending changes to Travis CI.

## Usage

Run commands from repository root. Successful image build acts as a smoke test.

Local installation of Docker is required.

### GCC

```sh
docker build \
  -f contrib/docker-build/gcc.Dockerfile \
  -t spectre/native-alg:gcc \
[ --build-arg=GCC_VERSION=7 ] \
  .
```

Valid values for `GCC_VERSION` as listed in <https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test>. Default shown.

### Clang

```sh
docker build \
  -f contrib/docker-build/clang.Dockerfile \
  -t spectre/native-alg:clang \
[ --build-arg=CLANG_VERSION=5.0 ] \
  .
```

Valid values for `CLANG_VERSION` as listed in <http://apt.llvm.org/trusty/dists/>. Default shown.

## Cleanup

Remember to remove excess images. You may consult image list with:

```sh
docker images
```

and remove selected ones with:

```sh
docker rmi IMAGE...
```
