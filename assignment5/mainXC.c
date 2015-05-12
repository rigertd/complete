#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dynamicArrayXC.h"
#include "toDoListXC.h"
#include "typeXC.h"

#define TESTHEAP
int main(int argc, const char * argv[])
{
  Task *task1, *task2, *task3, *task4, *task5, *task6, *task7, *task8, *task9, *task10;
	DynArr *mainList;
	int i;
	mainList = createDynArr(10);

	/* create tasks */
	task1 = createTask(9, "task 1");
	task2 = createTask(3, "task 2");
	task3 = createTask(3, "task 3");
	task4 = createTask(3, "task 4");
	task5 = createTask(3, "task 5");
	task6 = createTask(3, "task 6");
	task7 = createTask(8, "task 7");
	task8 = createTask(6, "task 8");
	task9 = createTask(1, "task 9");
	task10 = createTask(0, "task 10");

	/* add tasks to the dynamic array */
	addHeap(mainList, task1, compare);
	addHeap(mainList, task2, compare);
	addHeap(mainList, task3, compare);
	addHeap(mainList, task4, compare);
	addHeap(mainList, task5, compare);
	addHeap(mainList, task6, compare);
	addHeap(mainList, task7, compare);
	addHeap(mainList, task8, compare);
	addHeap(mainList, task9, compare);
	addHeap(mainList, task10, compare);

#ifdef TESTHEAP
	for(i = 0; i < sizeDynArr(mainList);i++)
	{
		Task *t = (Task *)getDynArr(mainList, i);
		printf("DynArr[%d] = %d\n", i, t->priority);
	}
		


	while(!isEmptyDynArr(mainList))
	{
		Task *t = (Task *)getMinHeap(mainList);
		printf("Val = %s ___%d\n", t->description, t->priority);
		removeMinHeap(mainList, compare);
	}
#endif

#ifdef TESTSORT

	printf("Before Sort Called \n");
	for(i = 0; i < sizeDynArr(mainList);i++)
			printf("DynArr[%d] = %d\n", i, ((Task*)getDynArr(mainList,i))->priority);


	/* sort tasks */
	sortHeap(mainList, compare);
	printf("After Sort Called \n");

	/* print sorted tasks from the dynamic array */
	for(i = 0; i < sizeDynArr(mainList);i++)
				printf("DynArr[%d] = %d\n", i, ((Task*)getDynArr(mainList,i))->priority);

	return 0;

#endif
}
