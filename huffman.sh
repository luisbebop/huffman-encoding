#!/bin/bash

# This simple script runs ./huffman with parameter $1
# and uses Dot to produce a pdf file with the
# Huffman tree and Evince to open the pdf.

echo "$#"
if [ "$#" -ne 1 ]; then
    ./main 
else
    ./main "$1"

fi
dot -Tpdf huffman.gv -o huffman.pdf
evince huffman.pdf