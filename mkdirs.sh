#!/bin/bash

DIR_NAME="batata"
DIR_NAME1="pepino"
DIR_NAME2="couve"
FILE_NAME1="mike"
FILE_NAME2="pp"
FILE_NAME3="ahm"

mkdir $DIR_NAME
cd $DIR_NAME
for i in {1..1000}
do
    mkdir -p "$DIR_NAME - $i"
    touch "$FILE_NAME1 - $i"
done

cd "$DIR_NAME - 1"
for i in {1..1000}
do
    mkdir -p "$DIR_NAME1 - $i"
    touch "$FILE_NAME2 - $i"
done

cd "$DIR_NAME1 - 1"
for i in {1..10000}
do
    mkdir -p "$DIR_NAME2 - $i"
    touch "$FILE_NAME3 - $i"
done

mkdir -p $DIR_NAME2