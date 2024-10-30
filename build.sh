#!/bin/bash

set -xe

gcc -Wall -Werror -ggdb main.c -o bin/main.out

bin/main.out