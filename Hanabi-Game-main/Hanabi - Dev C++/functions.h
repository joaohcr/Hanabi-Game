#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

int Sum(int arr[], int n)
{
	int i=0, sum=0;
	for(i=0; i<n; i++) {
		sum+=arr[i];
	}
	return sum;
}
int Random(int k)
{
	int n;
	srand(time(NULL));
	n=rand()%k;
	return n;
}
void Switch(int *v, int *w)
{
	int aux;
	aux=*v;
	*v=*w;
	*w=aux;
}
void SelectionSort(int *v, int n)
{
	int i, j, min;

	for(i=0;i<n-1;i++) {
		min=i;
		for(j=i+1; j<=n; j++) {
			if(v[j]<v[min])
				min=j;
		}
		if(min!=i) {
			Switch((v+i),(v+min));
		}
	}
}
void ClearScreen()
{
	int i=0, j=0;
	for(j=0; j<30; j++) {
		for(i=0; i<80; i++)
			showCharAt(120+i, 6+j, ' ');
	}
}
