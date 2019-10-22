#!/bin/bash

EXE="$1"
FIL="$2"

FILES=$FIL/*.in

REDB='\e[31m\e[1m'
YELB='\e[93m\e[1m'
NC='\033[0m'
NCB='\033[0m\e[1m'

for f in $FILES; do
  out=${f::-2}
  out="${out}out"
  out2=${f::-2}
  out2="${out2}err"
  printf "${NC}TESTCASE: $f\n"
	valgrind --leak-check=full -q 2>/dev/null "./$EXE" <$f 1>test.out 2>test.err
	err=$?
	diff -q $out test.out > /dev/null
	err1=$?
	diff -q $out2 test.err > /dev/null
	err2=$?
	if [[ $err1 == "0" && $err2 == "0" && $err == "0" ]]
	then
		printf "${YELB}OK\n"
	else
		printf "${REDB}ERROR\n"
	fi
done
