#!/bin/bash

clean=0
tests=""
valgrind=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            echo "Clean Build"
            clean=1
            shift
        ;;
        -t|--tests)
            echo "Running Tests"
            tests="run_tests"
            shift
        ;;
        -v|--valgrind)
            echo "Running valgrind"
            shift
            valgrind="$1"
            shift
        ;;
        *)
            echo "Unknown option: $1"
            shift
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

if [ "${valgrind}" != "" ]; then
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(echo $valgrind)
fi

popd
