#!/bin/bash

#make sure all directories are initialized
[ -d bingo ] || mkdir bingo
[ -d bingo/internals ] || mkdir bingo/internals

#compile source
gcc -o bingo/internals/bingo_game src/bingo_game.c -lncurses -lpanel && echo "build complete!"

#copy in necessary source files
cp src/bingo.sh bingo
cp src/wordlist.txt bingo/internals

#create zip for distribution
zip -r bingo.zip bingo > logfile.txt && echo "zip ready!" 

exit 0
