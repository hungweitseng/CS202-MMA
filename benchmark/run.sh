sudo insmod ../kernel_module/blockmma.ko
sudo chmod 777 /dev/blockmma

# Single accelerator, single process
# Launch an accelerator
./accelerators 1 &
./benchmark 128
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

./accelerators 1 &
./benchmark 256
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

./accelerators 1 &
./benchmark 512
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

./accelerators 1 &
./benchmark 1024
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

#./benchmark 1024
#./benchmark 2048
#./accelerators_control "0.0.0.0" "q" 27072
#./benchmark 8192

# Multiple accelerators, single process
# Launch an accelerator

./accelerators 4 &
./benchmark 256 
./benchmark 512
./benchmark 1024
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

sleep 10

# Multiple accelerators, multiple processes
# Launch an accelerator
./accelerators 4 &
./benchmark 256 &
./benchmark 256 &
./benchmark 256 &
./benchmark 256
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

./accelerators 4 &
./benchmark 1024 &
./benchmark 1024 &
./benchmark 1024 &
./benchmark 1024
./accelerators_control "0.0.0.0" "q" 27072
sleep 5

sudo rmmod blockmma
