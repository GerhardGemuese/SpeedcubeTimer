#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void make(char * b, char * a);
void countdown(void);

int main(void)
{
	int a = 0, res = 0;
	time_t t, begin, end;
	char moves[6] = { 'R','L','U','D','F','B' };
	char extra[2] = { '2',39 };
	char letters[25];
	srand((unsigned)time(&t));

	make(moves, letters);

	for (int i = 0; i < 25; ++i)			//final output of the scramble
	{
		printf("%c", letters[i]);
		if (rand() % 2 == 1)
			printf("%c", extra[rand() % 2]);
		printf(" ");
	}
	printf("\nstart the solve?\n");
	system("pause");
	countdown();
	begin = time(NULL);
	printf("\a\ngo!\n");
	system("pause");
	end = time(NULL);
	res = (int)end - (int)begin;
	printf("your time: %ds\n",res);
	system("pause");
	return 0;
}

void make(char * b, char * a)
{
	int ok = 0;

	for (int i = 0; i < 25; ++i)		//25 step pattern UNSORTED
		a[i] = b[rand() % 6];

	do									//sorting out repetition in this loop
	{
		for (int i = 0; i < 24; ++i)
		{
			if (a[i] == a[i + 1])		//assigning new value
				a[i] = b[rand() % 6];
		}
		ok = 0;
		for (int i = 0; i < 24; ++i)	//checking if there's more repetition
		{
			if (a[i] == a[i + 1])
				ok = 1;
		}
	} while (ok == 1);
}

void countdown(void)
{
	int a = 0;
	time_t t2, begin, end;
	begin = time(NULL);

	do							//whenever the value changes it prints that new value
	{
		end = time(NULL);
		t2 = end - begin;
		if (a != 15 - t2)
		{
			a = 15 - (int)t2;
			printf("%d ", a);
		}
	} while (a > 0);
}