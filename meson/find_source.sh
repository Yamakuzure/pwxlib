#!/bin/sh
find "$MESON_SOURCE_ROOT"/src/ -type f -name '*.h' -or -name '*.cpp' | sort -u
