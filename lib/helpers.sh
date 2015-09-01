#!/bin/bash

echo_fail() {
	if [ "$TERM" != "dumb" ];then
		echo -e "\e[31m$1"  1>&2 ; tput sgr0
	else
		echo -e "$1"  1>&2
	fi
}

check_parameter() {
	if [ "$#" -ne 2 ]; then
		echo "$1 is missing"
		exit 1
	fi
}

success_or_die() {
	if [ "$?" -ne "0" ]; then
		exit 1;
	fi
}

echo_status_ok() {
	if [ "$TERM" != "dumb" ];then
		echo -e " \e[32mOK($1)"; tput sgr0
	else
		echo -e " OK($1)"
	fi
}

echo_status_fail() {
	if [ "$TERM" != "dumb" ];then
		echo -e " \e[31mFAIL($1)"; tput sgr0
	else
		echo -e " FAIL($1)"
	fi
}

echo_status() {
	if [ "$1" -ne "0" ]; then
		echo_status_fail $1
	else
		echo_status_ok $1
	fi
}

len() {
	local _array=( $@ )
	echo ${#_array[@]}
}
