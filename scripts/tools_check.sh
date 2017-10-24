#!/bin/bash
# Simple script to list versons of critical dev tools

PREFIX=i386-elf-

bash --version | head -n1
echo -en "Binutils: `${PREFIX}ld -v | head -n1`\n"
make --version | head -n1
${PREFIX}gcc --version | head -n1
nasm -v | head -n1
tar --version | head -n1
diff --version | head -n1
#find --version | head -n1
#chown --version | head -n1
git --version | head -n1
ggdb --version | head -n1 #TODO
