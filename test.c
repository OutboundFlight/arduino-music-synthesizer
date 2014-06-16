#include <stdio.h>
#include <math.h>

typedef struct myint
{
	int a;
} myint;

void func2(myint *a)
{
	a->a = 5;
	printf("a: %d", a->a);
}

void func1(myint a[])
{
	printf("a: %d", a[0].a);
	func2(&a[0]);
	printf("a: %d", a[0].a);
}


int main (void)
{
	myint a[1];
	a[0].a = 1;
	func1(a);
	printf("a: %d", a[0].a);

}
