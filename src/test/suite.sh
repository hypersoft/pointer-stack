#!/bin/bash

# Micro Application Test Suite

# Copyright 2013, Triston J. Taylor, All rights reserved

# This is a bash script that reads a label for the test from stdin, then reads a line
# of shell code to test for success/fail. The process is repeated for each test.

# This is an advanced shell script, as each shell code is sourced into the shell,
# rather than evaluated.

# A label of `shell' causes testing to be bypassed, and code to be run. This is mostly
# useful for printing messages, modifying environment, etc..

# Generally, all you need to do is write your test application, and pipe the prescribed
# format of data into the shell script.

# The exit status reflects how many tests failed.

declare -i succeed=0 fail=0 count=0;

declare -i FATAL=0; # set this to 1 at the head of a test to break all further tests.
declare -i INDENT=0; # set this to the number of spaces you want test case output indented.
declare -i DEAD=0; # Internal use only, we use this to skip further testing, but count all failures.
declare -i SCRIPTING=0; # set this to 1 to enter a read, execute loop, from within a shell code section

declare ACTION='Test case:'

[[ -t 1 ]] && { # output is terminal... color it!
	declare failed="$(tput bold)$(tput setf 4)[failed]$(tput sgr0)" succeeded="$(tput bold)$(tput setf 2)[succeeded]$(tput sgr0)"
} || { # output other...
	declare failed='[failed]' succeeded='[succeeded]'
}

# You can call on the following from any code section to switch on/off fatalaties
error.fatal() {
	FATAL=1;
}

error.normal() {
	FATAL=0;
}

# You can call on the following from within a shell code section to switch on/off read execute loop
scripting.code() {
	SCRIPTING=1;
}

scripting.ends() {
	SCRIPTING=0;
}

while read label; do

	[[ -z "$label" ]] && { echo ''; continue; } # passthrough whitespace

	read -r code;

	if [[ "$label" == shell ]]; then
		if (( DEAD == 1 )); then continue; fi;
		# shell code section
		source <(echo "$code");
		while (( SCRIPTING )); do
			read -r code; . <(echo "$code");
		done;
		continue;
	fi;

	let count++;
 
	if (( DEAD == 1 )); then let fail++; continue; fi;

	if source <(echo "$code"); then

		# FATAL=0; # deprecated, must be manually reset to improve code flow
		printf '%*s' $INDENT;
		echo ${ACTION} "$label" $succeeded; let succeed++;

	else

		printf '%*s' $INDENT;
		echo ${ACTION} "$label" $failed;
		printf '\n%*s' $INDENT;
		echo "$code" == false\;; let fail++;

		if (( FATAL )); then 
			echo $'\nImperative test failure in' $label;
			let DEAD=1; # set all operations further, fail
		fi;

	fi;

	# make sure scripting is off for good measure, it only works from a shell section
	SCRIPTING=0;

done;

echo '';
echo $succeed/$count succeeded.
echo $fail/$count failed.
echo '';

exit $fail;

