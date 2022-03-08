#!/bin/bash

sudo insmod ../kernel_module/blockmma.ko
sudo chmod 777 /dev/blockmma

# Single accelerator, single process
# Launch an accelerator
echo "Part B - 128 (5%)"
./accelerators 1 &
./benchmark_bonus 128
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part B - 256 (5%)"
./accelerators 1 &
./benchmark_bonus 256
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part B - 512 (5%)"
./accelerators 1 &
./benchmark_bonus 512
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part B - 1024 (5%)"
./accelerators 1 &
./benchmark_bonus 1024
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part C1 - 2048 (5%)"
./accelerators 1 &
./benchmark_bonus 2048
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

# Multiple accelerators, single process
# Launch an accelerator

echo "Part C2 - 512 (5%)"
./accelerators 4 &
./benchmark_bonus 512
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part C2 - 1024 (5%)"
./accelerators 4 &
./benchmark_bonus 1024
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

echo "Part C2 - 2048 (5%)"
./accelerators 4 &
./benchmark_bonus 2048
rc=$?
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

if [ ${rc} -ne 0 ];
then
    echo "Test Failed"
    exit 1
fi

# Multiple accelerators, multiple processes
# Launch an accelerator
echo "Part D - 256 (5%)"
./accelerators 4 &
./benchmark_bonus 256 &
./benchmark_bonus 256 &
./benchmark_bonus 256 &
./benchmark_bonus 256
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

echo "Part D - up to 2048 (10%)"
./accelerators 4 &
./benchmark_bonus 256 &
./benchmark_bonus 512 &
./benchmark_bonus 1024 &
./benchmark_bonus 2048
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

sudo rmmod blockmma
