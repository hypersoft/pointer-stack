#!/bin/bash

# Micro Application Test Suite

# Copyright 2013, Triston J. Taylor, All rights reserved

# This is a bash script that reads a label for the test from stdin, then reads a line
# of shell code to test for success/fail. The process is repeated for each test.

# This is an advanced shell script, as each shell code is sourced into the shell,
# rather than evaluated.

# Generally, all you need to do is write your test application, and pipe the prescribed
# format of data into the shell script.

# The exit status reflects how many tests failed.

declare -i succeed=0 fail=0 count=0;

[[ -t 1 ]] && { # output is terminal... color it!
	declare failed="$(tput bold)$(tput setf 2)[failed]$(tput sgr0)" succeeded="$(tput bold)$(tput setf 2)[succeeded]$(tput sgr0)"
} || { # output other...
	declare failed='[failed]' succeeded='[succeeded]'
}

{ # everything in this block goes to stderr..

echo '';

while read label; do
	let count++;
	read code;
	source <(echo "$code") || {
		echo Test case: "$label" $failed;
		let fail++;
	} && {
		echo Test case: "$label" $succeeded;
		let succeed++;
	}
done;


echo '';
echo $succeed/$count succeeded.
echo $fail/$count failed.
echo '';

} >&2;

exit $fail;

