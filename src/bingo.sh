#!/bin/bash
installdir="internals"
currentdir=$(pwd)
if [ -f $installdir/wordlist.txt ] && [ -f $installdir/bingo_game ]; then
    cd $installdir
    shuf wordlist.txt > .tmplist
    ./bingo_game
    rm .tmplist
    cd $currentdir
else
    echo "Proper files don't exist! Make sure you have bingo_game and wordlist.txt in the same folder as bingo.sh!"
fi
exit 0
