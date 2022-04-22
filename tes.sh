make clean; make
echo 300
echo 2
mpirun -n 2 ./foxes-rabbits 300 6000 900 8000 200000 7 100000 12 20 9999 > potato.out ; diff potato.out larger-instances/r300-6000-900-8000-200000-7-100000-12-20-9999.out
echo 4
mpirun -n 4 ./foxes-rabbits 300 6000 900 8000 200000 7 100000 12 20 9999 > potato.out ; diff potato.out larger-instances/r300-6000-900-8000-200000-7-100000-12-20-9999.out

echo 4000
echo 2
mpirun -n 2 ./foxes-rabbits 4000 900 2000 100000 1000000 10 400000 30 30 12345 > potato.out ; diff potato.out larger-instances/r4000-900-2000-100000-1000000-10-400000-30-30-12345.out
echo 4
mpirun -n 4 ./foxes-rabbits 4000 900 2000 100000 1000000 10 400000 30 30 12345 > potato.out ; diff potato.out larger-instances/r4000-900-2000-100000-1000000-10-400000-30-30-12345.out

echo 100000
echo 2
mpirun -n 2 ./foxes-rabbits 100000 200 500 500 1000 3 600 6 10 1234 > potato.out ; diff potato.out larger-instances/r100000-200-500-500-1000-3-600-6-10-1234.out
echo 4
mpirun -n 4 ./foxes-rabbits 100000 200 500 500 1000 3 600 6 10 1234 > potato.out ; diff potato.out larger-instances/r100000-200-500-500-1000-3-600-6-10-1234.out

echo 20000
echo 2
mpirun -n 2 ./foxes-rabbits 20000 1000 800 100000 80000 10 1000 30 8 500 > potato.out ; diff potato.out larger-instances/r20000-1000-800-100000-80000-10-1000-30-8-500.out
echo 4
mpirun -n 4 ./foxes-rabbits 20000 1000 800 100000 80000 10 1000 30 8 500 > potato.out ; diff potato.out larger-instances/r20000-1000-800-100000-80000-10-1000-30-8-500.out
