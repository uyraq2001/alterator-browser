#!/bin/bash

for dir in "${@}" 
do
	for file in $dir*.desktop
	do
		if grep -q "\[X-Alterator\]" $file
		then
			echo $file
		fi
	done
done




#category=""
#while read i
#do
#	if [[ "$i" == "Categories="* ]]
#	then
#		category=$(cut -d = -f 2 <<<$i)
#	else
#		true
#	fi
#done < $1
#cat $category
