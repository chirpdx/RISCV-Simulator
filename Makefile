src_dir := $(shell pwd)/src/
tests_dir := $(shell pwd)/tests/
memfile := ../tests/all_alu.mem
spnum :=65536
pcnum :=0 

rvsim_verbose: compile_verbose
	cd $(src_dir); \
	./rvsimv.out -mem $(memfile) -sp $(spnum) -pc $(pcnum)

rvsim: compile
	cd $(src_dir); \
	./rvsim.out -mem $(memfile) -sp $(spnum) -pc $(pcnum)

compile_verbose:
	cd $(src_dir); \
	gcc -o rvsimv.out -D__VerboseDebug__ -D__verbose__ commondefs.h decodefun.c executefun.c mainsim.c utilityfunctions.c

compile:
	cd $(src_dir); \
	gcc -o rvsim.out commondefs.h decodefun.c executefun.c mainsim.c utilityfunctions.c

help: 
	echo "Avaliable Targets compile, compile_verbose, rvsim, rvsim_verbose"