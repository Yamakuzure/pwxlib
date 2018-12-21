#!/bin/bash

xType="$1"

if [[ "x" == "x$xType" ]]; then
	echo "Usage: $0 <type>"
	echo
	echo "Type can be one of:"
	echo "  release - Release build"
	echo "  debug   - Debug build"
	echo "  dbgthread - thread-debug build (many messages about)"
	echo "  asan      - Use address sanitizer (enables debug) (disables lsan)"
	echo "  lsan      - Use leak sanitizer    (enables debug) (also enables asan)"
	echo "  tsan      - Use thread sanitizer  (enables debug)"
	exit 0
fi

base_opts=""
if [[ "release" == "$xType" ]]; then
	base_opts="--buildtype=release -Ddebug=false -Ddebug-thread=false"
elif [[ "debug" == "$xType" ]]; then
	base_opts="--buildtype=debug -Ddebug=true -Ddebug-thread=false"
elif [[ "dbgthread" == "$xType" ]]; then
	base_opts="--buildtype=debug -Ddebug=true -Ddebug-thread=true"
elif [[ "asan" == "$xType" ]]; then
	export ASAN_OPTIONS=detect_leaks=0
	base_opts="--buildtype=debug -Ddebug=true -Ddebug-thread=false -Db_sanitize=address"
elif [[ "lsan" == "$xType" ]]; then
	export ASAN_OPTIONS=detect_leaks=1
	base_opts="--buildtype=debug -Ddebug=true -Ddebug-thread=false -Db_sanitize=address"
elif [[ "tsan" == "$xType" ]]; then
	base_opts="--buildtype=debug -Ddebug=true -Ddebug-thread=false -Db_sanitize=thread"
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
