#!/bin/bash

PHILO_PATH='./philo'

## colors

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

function display_help()
{
	printf "usage:\t./tester.sh nb_of_tests\n"
	exit
}

if [ "$#" -eq "0" ]
then
	display_help
fi

## check if philo file is here

philo_ok=$(ls $PHILO_PATH 2>/dev/null | wc -l)
if [ "$philo_ok" -eq "0" ]
then
	printf "philo needed, please update PHILO_PATH\n"
	exit
fi

## test function

function test1()
{
	error=0
    for i in `seq 1 $1`;
    do
		printf "\rTesting $PHILO_PATH 1 800 200 200\t\t%4d" $i
		res1=$(timeout 5s $PHILO_PATH 1 800 200 200 | grep "die" | wc -l)
		if [ "$res1" -ne "1" ]
		then
			error=$(($error + 1))
		fi
    done

	if [ "$error" -eq "0" ]
	then
		printf "\rTesting $PHILO_PATH 1 800 200 200\t\t${GREEN}[OK]${NC}\n"
	else
		printf "\rTesting $PHILO_PATH 1 800 200 200\t\t${RED}[KO] $error${NC}\n"
	fi
}

function test2()
{
	error=0
    for i in `seq 1 $1`;
    do
		printf "\rTesting $PHILO_PATH 5 800 200 200\t\t%4d" $i
		res2=$(timeout 5s $PHILO_PATH 5 800 200 200 | grep "die" | wc -l)
		if [ "$res2" -ne "0" ]
		then
			error=$(($error + 1))
		fi
	done

	if [ "$error" -eq "0" ]
	then
		printf "\rTesting $PHILO_PATH 5 800 200 200\t\t${GREEN}[OK]${NC}\n"
	else
		printf "\rTesting $PHILO_PATH 5 800 200 200\t\t${RED}[KO] $error${NC}\n"
	fi
}

function test3()
{
	error=0
    for i in `seq 1 $1`;
    do
		printf "\rTesting $PHILO_PATH 5 800 200 200 7\t\t%4d" $i
		res3=$(timeout 5s $PHILO_PATH 5 800 200 200 7 | grep "enough" | wc -l)
		if [ "$res3" -ne "1" ]
		then
			error=$(($error + 1))
		fi
    done

	if [ "$error" -eq "0" ]
	then
		printf "\rTesting $PHILO_PATH 5 800 200 200 7\t\t${GREEN}[OK]${NC}\n"
	else
		printf "\rTesting $PHILO_PATH 5 800 200 200 7\t\t${RED}[KO] $error${NC}\n"
	fi
}

function test4()
{
	error=0
    for i in `seq 1 $1`;
    do
		printf "\rTesting $PHILO_PATH 4 410 200 200\t\t%4d" $i
		res4=$(timeout 5s $PHILO_PATH 4 410 200 200 | grep "die" | wc -l)
		if [ "$res4" -ne "0" ]
		then
			error=$(($error + 1))
		fi
    done

	if [ "$error" -eq "0" ]
	then
		printf "\rTesting $PHILO_PATH 4 410 200 200\t\t${GREEN}[OK]${NC}\n"
	else
		printf "\rTesting $PHILO_PATH 4 410 200 200\t\t${RED}[KO] $error${NC}\n"
	fi
}

function test5()
{
	error=0
    for i in `seq 1 $1`;
    do
		printf "\rTesting $PHILO_PATH 4 310 200 100\t\t%4d" $i
		res5=$(timeout 5s $PHILO_PATH 4 310 200 100 | grep "die" | wc -l)
		if [ "$res5" -ne "1" ]
		then
			error=$(($error + 1))
		fi
    done

	if [ "$error" -eq "0" ]
	then
		printf "\rTesting $PHILO_PATH 4 310 200 100\t\t${GREEN}[OK]${NC}\n"
	else
		printf "\rTesting $PHILO_PATH 4 310 200 100\t\t${RED}[KO] $error${NC}\n"
	fi
}

function estimated_time()
{
	tot=$((16*$1))
	min=$((tot/60))
	sec=$((tot-60*min))
	printf "Estimated time : $min minutes and $sec seconds\n"
}

## main

re='^[0-9]+$'
if [[ $1 =~ $re ]]
then
	estimated_time $1
	test1 $1
	test2 $1
	test3 $1
	test4 $1
	test5 $1
else
	display_help
fi
