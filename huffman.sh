#!/bin/bash

# This simple script runs ./main with parameter $1
# and uses Dot to produce a png file with the
# Huffman tree.

if [ "$#" -ne 1 ]; then
    ./main 
else
    ./main "$1"
fi

# Use Dot to produce the png file
dot -Tpng huffman.gv -o huffman.png

# Open the image
eog huffman.png