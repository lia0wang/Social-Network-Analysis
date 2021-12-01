#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

make testFloydWarshall || exit

run_test()
{
	i="$1"
	rm -f "../outputs/FloydWarshallTests/$i.out"
	./testFloydWarshall "../inputs/$i.in" > "../outputs/FloydWarshallTests/$i.out"
	
	if [ ! -f "../outputs/FloydWarshallTests/$i.exp" ]
	then
		echo -e "=========== ${YELLOW}[$i] No Expected Output Available${NC} ==========="
		return
	fi
	
	r="$(diff "../outputs/FloydWarshallTests/$i.out" "../outputs/FloydWarshallTests/$i.exp")"
	
	if [ "$r" = "" ]
	then
		echo -e "=========== ${GREEN}[$i] Output Matches${NC} ==========="
	else
		echo -e "=========== ${RED}[$i] Output Mismatch${NC} ==========="
		echo -e "${RED}Your output:${NC}"
		cat "../outputs/FloydWarshallTests/$i.out"
		echo -e "${RED}Expected output:${NC}"
		cat "../outputs/FloydWarshallTests/$i.exp"
		echo -e "${RED}Your output in: ../outputs/FloydWarshallTests/$i.out${NC}"
		echo -e "${RED}Expected output in: ../outputs/FloydWarshallTests/$i.exp${NC}"
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
	echo "usage: $0 <test number (1 for ../inputs/1.in, etc.)>"
	exit 1
fi

