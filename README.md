# Simmulated Annealing for VRPTW problem

![CI status](https://github.com/dziulek/simmulated_annealing/actions/workflows/c-cpp.yml/badge.svg)
![CI status](https://github.com/dziulek/simmulated_annealing/actions/workflows/test.yml/badge.svg)

## Overview
This is implementtion of simmulated annealing metaheuristic for Vehicle Routing Problem with Time Windows ([VRPTW](https://www.hindawi.com/journals/mpe/2012/104279/)).

Simple demonstration how algorithm creates and optimizes initial solution.
![](annealingDemo.gif)

Bigger view depicts actual solution, while smaller best solution found so far.

## Build
To build project clone repository with submodules.
```
cd simmulated_annealing
mkdir build
cd build
cmake ..
make
```
To run algorithm without UI
```
./src/annealing_run
```
To run with UI
```
./src/simpleUI_run
```