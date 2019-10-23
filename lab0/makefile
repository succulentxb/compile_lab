#run: main
#	./main

./main: main.o prog1.o slp.o util.o answer.o
	cc -g main.o prog1.o slp.o util.o answer.o -o main

main.o: main.c slp.h util.h
	cc -g -c main.c

answer.o: answer.c answer.h slp.h util.h
	cc -g -c answer.c

prog1.o: prog1.c slp.h util.h
	cc -g -c prog1.c

slp.o: slp.c slp.h util.h
	cc -g -c slp.c

util.o: util.c util.h
	cc -g -c util.c

clean: 
	rm -f a.out util.o prog1.o slp.o answer.o main.o main
