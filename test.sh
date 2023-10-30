#!/bin/bash

for file in "."/*_test
do
    chmod +x $file
    if [ -f "$file" ] && [ -x "$file" ]; then
        echo "test file: $file"
        $file
    else
        echo "$file not exit or can not be run"
    fi
done