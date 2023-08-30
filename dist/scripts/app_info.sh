#!/bin/bash

[[ "$1" =~ /usr/share/applications/.*\.desktop ]] && cat $1
