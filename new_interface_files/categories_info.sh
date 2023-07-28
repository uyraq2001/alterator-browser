#!/bin/bash

[[ "$1" =~ /usr/share/alterator/categories/.*\.directory ]] && cat $1
