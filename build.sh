#!/bin/sh
set -xe

cc -Wall -Wextra -g -o 3D 3D.c -L/usr/local/lib -lraylib -lm 
cc -Wall -Wextra -g -o 2D 2D.c -L/usr/local/lib -lraylib -lm 
# cc -Wall -Wextra -g -o number-distribution number-distribution.c -L/usr/local/lib -lraylib -lm 
