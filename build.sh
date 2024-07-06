#!/bin/bash

clean=0
tests=""

for arg in "$@"
do
    case $arg in
        -c|--clean)
            echo "Clean Build"
            clean=1
        ;;
        -t|--tests)
            echo "Running Tests"
            tests="run_tests"
        ;;
        *)
            echo "Unknown option: $arg"
        ;;
    esac
done

if [ $clean == 1 ]; then
    rm -rf build
    cmake -S ./ -B build
fi
pushd "build"

if [ "${tests}" != "" ]; then
    make -j$(nproc) "${tests}"
else
    make -j$(nproc)
fi
popd
