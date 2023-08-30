#!/bin/bash

for dir in "${@}" 
do
	for file in $dir*.desktop
	do
		if grep -q "\[X-Alterator Interface .*\]" $file
		then
			echo $file
		fi
	done
done
