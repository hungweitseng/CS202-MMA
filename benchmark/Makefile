all: accelerators accelerators_control benchmark benchmark_bonus

# benchmark_single_proc_multi_accel benchmark_multi_proc_multi_accel

benchmark: benchmark.c
	$(CC) -g -o benchmark benchmark.c -lblockmma

benchmark_bonus: benchmark_bonus.c
	$(CC) -g -o benchmark_bonus benchmark_bonus.c -lblockmma

accelerators: accelerators.c
	$(CC) -g -o accelerators accelerators.c -lblockmma

accelerators_control: accelerators_control.c
	$(CC) -g -o accelerators_control accelerators_control.c


clean:
	rm -f *.o accelerators accelerators_control benchmark 


.PHONY: all clean
