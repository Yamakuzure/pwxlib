#!/bin/bash

xType="$1"

if [[ "x" == "x$xType" ]]; then
	echo "Usage: $0 <type> [extra options for meson]"
	echo
	echo "Type can be one of:"
	echo "  release - Release build"
	echo "  debug   - Debug build"
	echo "  asan      - Use address sanitizer (enables debug) (disables lsan)"
	echo "  lsan      - Use leak sanitizer    (enables debug) (also enables asan)"
	echo "  tsan      - Use thread sanitizer  (enables debug)"
	exit 0
fi

shift 1
xExtra="$@"

base_opts=""
if [[ "release" == "$xType" ]]; then
	base_opts="--buildtype=release -Ddebug=false -Doptimization=2"
elif [[ "debug" == "$xType" ]]; then
	base_opts="--buildtype=debug -Ddebug=true -Doptimization=g"
elif [[ "asan" == "$xType" ]]; then
	export ASAN_OPTIONS=detect_leaks=0
	base_opts="--buildtype=debug -Ddebug=true -Db_sanitize=address -Dsmall_tests=true -Doptimization=g"
elif [[ "lsan" == "$xType" ]]; then
	export ASAN_OPTIONS=detect_leaks=1
	base_opts="--buildtype=debug -Ddebug=true -Db_sanitize=address -Dsmall_tests=true -Doptimization=g"
elif [[ "tsan" == "$xType" ]]; then
	base_opts="--buildtype=debug -Ddebug=true -Db_sanitize=thread -Dsmall_tests=true -Doptimization=0"
else
        echo "Type \"$xType\" is unknown"
        exit 1
fi

rm -rf build

meson $base_opts             \
        --prefix=/usr        \
        -Dtests=true         \
        -Dinstall-tests=true \
        -Dtorture=true       \
        -Dwarning_level=3    \
        $xExtra              \
        build

if [[ 0 -ne $? ]]; then
        echo -e "\n----------------\nPlease fix and re-run\n----------------"
        exit 1
fi

echo -n "Do the build? [y/N] : "
read xAns
if [[ "xy" != "x$xAns" ]]; then
        echo "k thx bye"
        exit 0
fi

ninja -C build

if [[ 0 -ne $? ]]; then
        echo -e "\n----------------\nPlease fix and re-run\n----------------"
        exit 1
fi

echo -n "Do the install? [y/N] : "
read xAns
if [[ "xy" != "x$xAns" ]]; then
        echo "k thx bye"
        exit 0
fi

ninja -C build install
