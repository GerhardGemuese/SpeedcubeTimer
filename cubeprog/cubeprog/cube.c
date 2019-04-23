#define _CRT_SECURE_NO_WARNINGS
#define MAX 10000
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

void fflushstdin(void);
int read(FILE *file);
void make(const char b[], char a[]);
double average(int x, int n);
int best(int x, int n);
void stats(int *n, FILE *file);
void output(char pat[], char x[], int *n, FILE *file);
void countdown(void);
int stopw(int n);
void opt(char in, FILE *file, int *n);
void opt_d(FILE *file);
char test(FILE *file, int *n);

const char letters[6] = { 'R','L','U','D','F','B' };
int data[MAX];
/*struct data
{
	int			savetime;
	struct data	*next;
};*/

int main(void)
{
	int a = 0, res = 0, n = 0;
	int * s = &res;
	time_t t;
	char extra[2] = { '2',39 }, moves[25];
	FILE *solves;
	srand((unsigned)time(&t));
	solves = fopen("solves.cube", "a+b");				//opens file in binary mode
	if (solves == NULL) {
		fprintf(stderr ,"opening error.");
		exit(1);
	}
	stats(&n, solves);
	do
	{
		make(letters, moves);											//creates the scramble
		output(moves, extra, &n, solves);
		printf("\nstart the solve?\n");
		system("pause");
		countdown();													//counts down from 15 for inspection time
		res = stopw(n);
		fseek(solves, 0, SEEK_END);
		fprintf(solves, "%d\r", res);									// '\r' needed for new line. why?
		printf("your time: %ds\nwould you like to go again? [y / n]\ts: show statistics\td: reset statistics\tl: delete this time\t", res);
	} while (test(solves, &n) == 'y');
	_fcloseall();
	return 0;
}

void fflushstdin(void)
{
	int c;
	while ((c = fgetc(stdin)) != EOF && c != '\n');
}

int read(FILE *file)
{
	int i = 0;
	rewind(file);
	while (feof(file) == 0) {
		if (fscanf(file, "%d", &data[i]) < 0)
			data[i] = 0;
		if (data[i] == 0)
		{}
		else
			i++;
	}
	return i;
}

void make(const char b[], char a[])
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
		for (int i = 0; i < 24; ++i)	//checking if there's still repetition
		{
			if (a[i] == a[i + 1])
				ok = 1;
		}
	} while (ok == 1);
}

double average(int x, int n)
{
	if (n == 0)
		return 0;
	else
	{
		double avg = 0;
		if (x < 0)
			x = 0;
		for (int i = x; i < n; i++)
			avg += data[i];
		avg /= (double)(n-x);
		return avg;
	}
}

int best(int x, int n)
{
	if (n < 0)
		return 0;
	else
	{	
		int max = data[0];
		if (x < 0)
			x = 0;
		for (int i = x; i < n; i++)				
			if (data[i] < max)
				max = data[i];
		return max;
	}
}

void stats(int *n, FILE *file)
{
	*n = read(file);														//copying times from file into data array
	char words[4][30] = { "avg","best","avg of 5","best of 5" };
	printf("number of solves: %d\n%9s: %9.2lfs\t\t%9s: %9.2lfs\n%9s: %9ds\t\t%9s: %9ds\n", *n
		, words[0], average(0, *n), words[2], average(*n - 5, *n)
		, words[1], best(0, *n), words[3], best(*n - 5, *n));
}

void output(char pat[], char x[], int *n, FILE *file)
{
	for (int i = 0; i < 25; ++i)												//final output of the scramble
	{
		printf("%c", pat[i]);
		if (rand() % 2 == 1)
			printf("%c", x[rand() % 2]);
		printf(" ");
	}
}

void countdown(void)
{
	int a = 15;
	while (a>0 && _kbhit()==0)
	{
		printf("%d\t", a);
		a--;
		Sleep(1000);					// = 1second
	}
}

int stopw(int n)
{
	time_t begin, end;
	int t = 0;
	begin = time(NULL);
	printf("\a\ngo!\n");
	system("pause");
	end = time(NULL);
	t = (int)end - (int)begin;
	if (t < best(0, n))
		printf("new best time!\n");
	return t;
}

void opt_d(FILE *file)			//resets statistics
{
	char confirm = 0;
	printf("would you like to reset? [y / n]");
	do
	{
		fflushstdin();
		scanf("%c", &confirm);
		if (confirm != 'y' && confirm != 'n')
			printf("wrong input.");
	} while (confirm != 'y' && confirm != 'n');
	if (confirm == 'y')
	{
		fclose(file);
		if (remove("solves.cube") == 0)
			printf("reset successful.\n");
		else
		{
			fprintf(stderr, "reset error.");
			exit(2);
		}
		file = fopen("solves.cube", "a+b");
		if (file == NULL)
		{
			fprintf(stderr, "opening error.");
			exit(1);
		}
	}
}

void opt(char in, FILE *file, int *n)
{
	long off = 0;
	if (in == 'd')
		opt_d(file);
	else if (in == 's') {		//displays statistics
		printf("\n");
		stats(&*n, file);
	}
	else if (in == 'l') {			//supposed to overwrite last time. NOT YET FUNCTIONING!
		//off = ftell(file);
		//fseek(file, off, SEEK_SET);
		//printf("%d\n", ftell(file));
		//fprintf(file,"%d\r",0);
		data[*n - 1] = 0;
		fwrite(data, sizeof(int), *n, file);
		printf("deleted.\n");
	}
	printf("would you like to go again? [y / n]\ts: show statistics\td: reset statistics\tl: delete last time\t");
}

char test(FILE *file, int *n)
{
	char input = 0;
	do
	{
		scanf("%c", &input);
		if (input != 'y' && input != 'n' && input != 'd' && input != 's' && input != 'l')
			printf("wrong input.");
		if (input == 'd' || input == 's' || input == 'l')
			opt(input, file, &*n);
		fflushstdin();
	} while (input != 'y' && input != 'n');
	printf("\n");
	return input;
}