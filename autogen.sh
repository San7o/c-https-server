#!/bin/sh

# Run this script to setup or regenerate the project

aclocal
autoconf
libtoolize --force --copy
automake --add-missing

echo "Autotools regeneration complete."
