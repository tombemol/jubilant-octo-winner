#!/usr/bin/env sh
set -eu
mkdir -p build
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.c -o build/livraria
printf 'Compilado em build/livraria\n'
