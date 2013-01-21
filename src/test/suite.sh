#!/bin/bash

# Micro Application Test Suite

# Copyright 2013, Triston J. Taylor, All rights reserved

# This is a bash script that reads a label for the test from stdin, then reads a line
# of shell code to test for success/fail. The process is repeated for each test.

# A label of `shell' causes testing to be bypassed, and code to be run. This is mostly
# useful for printing messages, modifying environment, etc..

# This is an advanced shell script, as each shell code is sourced into the shell,
# rather than evaluated.

# Generally, all you need to do is write your test application, and pipe the prescribed
# format of data into the shell script.

# The exit status reflects how many tests failed.

declare -i succeed=0 fail=0 count=0;

declare -i FATAL=0; # set this to 1 at the head of a test to break all further tests.

declare -i DEAD=0; # Internal use only, we use this to skip further testing, but count all failures.

[[ -t 1 ]] && { # output is terminal... color it!
	declare failed="$(tput bold)$(tput setf 4)[failed]$(tput sgr0)" succeeded="$(tput bold)$(tput setf 2)[succeeded]$(tput sgr0)"
} || { # output other...
	declare failed='[failed]' succeeded='[succeeded]'
}

echo '';

while read label; do

	read code;

	if [[ "$label" == shell ]]; then
		if (( DEAD == 1 )); then continue; fi;
		source <(echo "$code"); continue;
	fi;

	let count++;
 
	if (( DEAD == 1 )); then let count++; let fail++; continue; fi;

	if source <(echo "$code"); then

		echo Test case: "$label" $succeeded; let succeed++;

	else

		echo Test case: "$label" $failed; let fail++;

		if (( FATAL )); then 
			FATAL=0; # reset FATAL flag
			echo $'\nImperative test failure in' $label;
			let DEAD=1; # set all operations further, fail
		fi;

	fi;

done;

echo '';
echo $succeed/$count succeeded.
echo $fail/$count failed.
echo '';

exit $fail;

