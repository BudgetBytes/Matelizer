#!/bin/sh
set -xe

cc -Wall -Wextra -g -o main main.c -L/usr/local/lib -lraylib -lm 
cc -Wall -Wextra -g -o number-distribution number-distribution.c -L/usr/local/lib -lraylib -lm 
