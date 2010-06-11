INCL=-I. -I/usr/local/cuda/lib/
LINK=-L. -L/usr/local/cuda/lib/
LIB=-lcuda -lcudart
CFLAG= -fopenmp -O3

test: cuda garchlik_core.o
	g++ -c $(CFLAG) garchcore.cpp $(INCL) -o garchcore.o
	g++ -c $(CFLAG) test.cpp $(INCL) -o test.o
	g++ $(CFLAG) garchlik_core.h garchcore.o garchlik_core.o test.o $(INCL) $(LINK) $(LIB) -o test

cuda:
	export OMP_NUM_THREADS=1
	nvcc -c -O3 --keep -use_fast_math garchlik_core.cu $(INCL)
	# -Xcompiler -fopenmp -DTHRUST_DEVICE_BACKEND=THRUST_DEVICE_BACKEND_OMP -lcudart -lgomp
