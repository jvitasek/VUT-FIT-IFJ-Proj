#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
void SiftDown(int A[], left, right)
{
	int temp=0;
	int i=0;
	int j=0;
	bool cont = false;

	i=left;
	j=2*i;
	temp = A[i];
	if(j<=right)
		{cont = true;}
	while(cont)
	{
		if(j<right)
		{
			if(A[j]<A[j+1])
			{
				j=j+1;
			}
			if(temp>=A[j])
			{
				cont=false;
			}
			else
			{
				A[i] = A[j];
				i=j;
				j=2*i;
				if(j<=right)
				{cont = true;}
			}

		}
	}
A[i]=temp;
return;
}

void HeapSort(int A[])
{
	int i=0;
	int left=0;
	int right=0;
	int pom=0;
	int N=5;


	right = N;
	left = N/2;
	printf("%d\n",left );
	i=left;
	for (i;i>=1;i--)
	{
	
		SiftDown(A,i, right);
	}
	right=N;
	for (right; right>=2; N--)
	{
		
		pom=A[1];
		A[1]=A[right];
		A[right]=pom;
		SiftDown(A,1,right-1);


	}

}

