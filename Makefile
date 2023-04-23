main.exe: main.c arrayList.o graph.o
	gcc -std=c99 main.c arrayList.o graph.o -o main.exe

arrayList.o: arrayList.h arrayList.c
	gcc -std=c99 arrayList.c -c

graph.o: graph.h graph.c
	gcc -std=c99 graph.c -c
