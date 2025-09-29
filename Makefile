CC=gcc -w -lm -lgsl -lgslcblas -O3

all: intf clean

intf: intf.c calc.o data.o list.o loop.o plot.o rng.o run.o save.o set.o step.o
	${CC} -o intf intf.c calc.o data.o list.o loop.o plot.o rng.o run.o save.o set.o step.o

calc.o: calc.c
	${CC} -o calc.o -c calc.c

data.o: data.c
	${CC} -o data.o -c data.c

list.o: list.c
	${CC} -o list.o -c list.c

loop.o: loop.c
	${CC} -o loop.o -c loop.c

plot.o: plot.c
	${CC} -o plot.o -c plot.c

rng.o: rng.c
	${CC} -o rng.o -c rng.c

run.o: run.c
	${CC} -o run.o -c run.c

save.o: save.c
	${CC} -o save.o -c save.c

set.o: set.c
	${CC} -o set.o -c set.c

step.o: step.c
	${CC} -o step.o -c step.c

clean:
	rm *.o
