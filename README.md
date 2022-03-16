# RISCV-Simulator
Make targets
1. make rvsim                    (silent)
2. make rvsim_verbose            (verbose without singelstepping)
3. make rvsim_singleverbose      (verbose along with extra verbose and singlestepping)

Possible command line arguments, if not default

Makefile    Target exe
pcnum       -pc
spnum       -sp
memfile     -mem

e.g
make rvsim pcnum=c spnum=65536 memfile=../tests/rvn.mem


##github repo
* [github repo](https://github.com/chirpdx/RISCV-Simulator)
