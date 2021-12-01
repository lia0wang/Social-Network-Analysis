#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

make testGirvanNewman || exit

run_test()
{
	i=$1
	rm -f "../outputs/GirvanNewmanTests/$i.out"
    ./testGirvanNewman "../inputs/$i.in" | sort -n > "../outputs/GirvanNewmanTests/$i.out"
    
    if [ ! -f "../outputs/GirvanNewmanTests/$i.exp" ]
	then
		echo -e "=========== ${YELLOW}[$i] No Expected Output Available${NC} ==========="
		return
	fi
	
    r="$(diff -B "../outputs/GirvanNewmanTests/$i.out" "../outputs/GirvanNewmanTests/$i.exp")"

    if [ "$r" = "" ]
    then
        echo -e "=========== ${GREEN}[$i] Output Matches${NC} ==========="
    else
        echo -e "=========== ${RED}[$i] Output Mismatch${NC} ==========="
        echo -e "${RED}Your output:${NC}"
		cat "../outputs/GirvanNewmanTests/$i.out"
		echo -e "${RED}Expected output:${NC}"
		cat "../outputs/GirvanNewmanTests/$i.exp"
        echo -e "${RED}Your output in: ../outputs/GirvanNewmanTests/$i.out${NC}"
        echo -e "${RED}Expected output in: ../outputs/GirvanNewmanTests/$i.exp${NC}"
    fi
}

if [ $# -eq 1 ]
then
	if [ ! -f "../inputs/$1.in" ]
	then
		echo "error: ../inputs/$1.in does not exist"
		exit 1
	fi
	run_test $1
elif [ $# -eq 0 ]
then
	for f in ../inputs/*.in
	do
		i=$(basename "$f" .in)
		run_test "$i"
	done
else
	echo "usage: $0 <test number (1 for ..inputs/1.in, etc.)>"
	exit 1
fi

