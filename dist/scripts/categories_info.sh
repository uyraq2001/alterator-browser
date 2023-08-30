#!/bin/bash

file_name="/usr/share/alterator/desktop-directories/miscellaneous.directory"
is_legacy=true

for dir in "${@:1:$#-1}"
do
    for file in $dir*.directory
    do
        if grep -qs "\[X-Alterator Category ${@: -1}\]" $file
        then
            file_name=$file
            is_legacy=false
        fi
        if grep -Eqs "\bX-Alterator-Category=${@: -1}\b" $file
        then
            file_name=$file
        fi
    done
done

if [[ "$is_legacy" == true ]]
then
	cat "$file_name"
else

    IFS=$'\r\n'
    GLOBIGNORE='*'
    command eval 'lines=($(cat "$file_name"))'

    ans=()
    ok=false

    for (( i=0; i<${#lines[@]}; i++))
    do
        if [[ ${lines[i]} =~ ^\[X-Alterator\ Category\ "${@: -1}"\]$ ]]
        then
            ok=true
        else
            [[ ${lines[i]} =~ ^\[.*\]$ ]] && ok=false
        fi
        if [ "$ok" == true ]
        then
            echo "${lines[i]}"
        fi
    done

fi
