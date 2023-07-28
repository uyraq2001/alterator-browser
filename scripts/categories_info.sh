#!/bin/bash

file_name="/usr/share/alterator/categories/Miscellaneous.directory"

for dir in "${@:1:$#-1}"
do
        for file in $dir*.directory
        do
                if grep -q "\[X-Alterator Category ${@: -1}\]" $file
                then
                        file_name=$file
                fi
        done
done

IFS=$'\r\n'
GLOBIGNORE='*'
command eval 'lines=($(cat "$file_name"))'

ans=()
ok=false

for (( i=0; i<${#lines[@]}; i++))
do
    if [[ ${lines[i]} =~ ^\[X-Alterator\ Category\ "${@: -1}"\]$ ]]; then
        ok=true
    else
        [[ ${lines[i]} =~ ^\[.*\]$ ]] && ok=false
    fi
    if [ "$ok" == true ]
    then
        echo "${lines[i]}"
    fi
done
