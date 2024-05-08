#!/bin/bash

export DISPLAY=$(route.exe print | grep 0.0.0.0 | head -1 | awk '{print $4}'):0.0
g++ -c pacman.cpp
g++ pacman.o -o app -pthread -lsfml-graphics -lsfml-window -lsfml-system
./app