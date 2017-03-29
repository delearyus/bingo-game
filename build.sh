#!/bin/bash

gcc -o bingo/internals/bingo_game src/bingo_game.c -lncurses -lpanel && echo "build complete!" #compile source
zip -r bingo.zip bingo > logfile.txt && echo "zip ready!" #create zip for distribution

exit 0
