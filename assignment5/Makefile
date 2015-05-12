all: prog1 prog2 progxc

prog1: dynamicArray.o main.o toDoList.o
	gcc -g -Wall -std=c99 -o prog1 dynamicArray.o toDoList.o main.o
	
prog2: dynamicArray.o main2.o toDoList.o
	gcc -g -Wall -std=c99 -o prog2 dynamicArray.o toDoList.o main2.o
	
progxc: mainXC.o dynamicArrayXC.o toDoListXC.o
	gcc -g -Wall -std=c99 -o progxc dynamicArrayXC.o toDoListXC.o mainXC.o
	
main.o: main.c dynamicArray.h type.h toDoList.h
	gcc -g -Wall -std=c99 -c main.c
	
main2.o: main2.c dynamicArray.h type.h toDoList.h
	gcc -g -Wall -std=c99 -c main2.c
	
dynamicArray.o: dynamicArray.c dynamicArray.h
	gcc -g -Wall -std=c99 -c dynamicArray.c
	
toDoList.o: toDoList.c toDoList.h
	gcc -g -Wall -std=c99 -c toDoList.c

mainXC.o: mainXC.c dynamicArrayXC.h typeXC.h toDoListXC.h
	gcc -g -Wall -std=c99 -c mainXC.c
	
dynamicArrayXC.o: dynamicArrayXC.c dynamicArrayXC.h
	gcc -g -Wall -std=c99 -c dynamicArrayXC.c
	
toDoListXC.o: toDoListXC.c toDoListXC.h
	gcc -g -Wall -std=c99 -c toDoListXC.c
	
clean:	
	rm dynamicArray.o
	rm toDoList.o
	rm main.o
	rm main2.o
	rm dynamicArrayXC.o
	rm toDoListXC.o
	rm mainXC.o
	rm prog1
	rm prog2
	rm progxc
