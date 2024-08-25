#! /bin/sh

# This script is used to create a new release of the Engine

if [ $# -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

# Create the release directory
rm -rf release
mkdir -p release

# Copy the files to the release directory
tar -czf release/brenta-engine-docs-v$1.tar.gz docs/html
tar -czf release/brenta-engine-src-v$1.tar.gz engine utils ecs tests examples CMakeLists.txt README.md LICENSE flake.nix flake.lock lib Makefile include
