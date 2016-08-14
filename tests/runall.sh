#! /usr/bin/env bash

for file in build/tests/*; do
    if [[ -f $file ]]; then
        ./$file
    fi
done
