#!/bin/sh

# Wrappers are provided from the call
for x in $@ ; do
        echo "$x" | sed -e "s,$MESON_SOURCE_ROOT,..,g"
done

# Headers must be found and go first
find "$MESON_SOURCE_ROOT"/src/ -type f -name '*.h' | \
        sort -u | \
        sed -e "s,$MESON_SOURCE_ROOT,..,g"

# Followed by the source files
find "$MESON_SOURCE_ROOT"/src/ -type f -name '*.cpp' | \
        sort -u | \
        sed -e "s,$MESON_SOURCE_ROOT,..,g"
