#!/bin/bash

category=""
while read i
do
	if [[ "$i" == "Categories="* ]]
	then
		category=$(cut -d = -f 2 <<<$i)
	else
		true
	fi
done < $1
cat $category
