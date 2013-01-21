#!/bin/bash

# Micro Application Test Suite

# Copyright 2013, Triston J. Taylor, All rights reserved

# To use this script, simply pipe in a label for your test, and issue a shell statement
# that should succeed for each test.

declare -i succeed=0 fail=0 count=0;

while read label; do
	let count++;
	read code;
	source <(echo $code) || {
		echo test case $label: failed;
		let failed++;
	} && {
		echo test case $label: succeeded;
		let succeed++;
	}
done;

echo '';
echo $success/$count succeeded.
echo $failed/$count failed.
echo '';

exit $failed;

