#!/bin/bash

set -xe

gcc -Wall -Werror -ggdb -I. main.c -o bin/main.out -lm

bin/main.out files/oracle_depicted_in_the_matrix.png