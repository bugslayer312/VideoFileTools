#!/bin/sh

build_type=Release
build_tests=OFF
build_make_only=0

for param in $*; do
  name=${param%%=*}
  value=${param#*=}
  case "$name" in
    --type) build_type=$value;;
    --tests) build_tests=$value;;
    --make_only) build_make_only=1;;
  esac
done

if [ $build_make_only -eq 0 ]; then
  for dir in build output; do
    if [ ! -d $dir ]; then
      mkdir $dir
    fi
  done

  for dir in build/$build_type output/$build_type; do
    if [ ! -d $dir ]; then
      mkdir $dir
    fi
    rm -r $dir/* 2> /dev/null
  done
fi

cd build/$build_type
if [ $build_make_only -eq 0 ]; then
  cmake  -DCMAKE_BUILD_TYPE="$build_type"  -DBUILD_GTESTS="$build_tests" ../../src
fi
make
