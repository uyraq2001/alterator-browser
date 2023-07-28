#!/bin/bash

for dir in "${@}" 
do
	for file in $dir*.directory
	do
		if grep -q "\[X-Alterator Category .*\]" $file
		then
			echo $file
		fi
	done
done
