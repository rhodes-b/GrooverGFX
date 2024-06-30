cmake -S ./ -B build
pushd "build"
make -j$(nproc)
popd
