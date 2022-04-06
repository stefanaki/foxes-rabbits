# Parallel and Distributed Computing - IST 2021/2022

## Introduction

Student project that simulates a simple ecosystem which consists of three types of elements, rocks, rabbits and foxes. The goal was the computation of how the population of the ecosystem evolves through the generations of the simulation, according to the rules described in the [project announcement](doc/projPDC2122-foxesRabbits.pdf). Three different implementations where developed, a serial version, an OpenMP parallel version with shared memory and an MPI version with distributed memory.

## Group 10

-   Nelson Trindade – 93743

-   André Ribeiro – 104083

-   Georgios Stefanakis – 104596

## Report

Report can be found in source folder with the name `g10_omp_report.pdf`, or you can find it here: [g10_omp_report.pdf](doc/g10_omp_report.pdf)

## Scripts

We can use the test script `test.sh` and the provided instances to assert the correctness of the results in an automatic way.

You can run the script with or without arguments:

-   `./test.sh ` - smaller test cases only
-   `./test.sh large-only` - larger test cases only
-   `./test.sh all` - both large and smaller test cases

The script `test_thread.sh` makes a run of all tests, modifying number of threads the computer will use to run the tests, for each set of tests. The arguments used in `test.sh` can be applied to this script.
