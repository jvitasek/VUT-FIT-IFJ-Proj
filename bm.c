#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))



char MatchJumpP[255];
char CharJumpP[255];
void ComputeJumps(*p, char CharJump[255])
{
	int i, a,k;
	for(a = 0; a < 256; a++)
	{
		CharJump[a] = strlen(p);
	}

	for(k = 1; k < strlen(p); k++)
	{
		CharJump[p[k]] = strlen(p)-k;
	}
	
}

void ComputeMatchJump(*s, char MatchJump[255])
{

	int k, q, qq, m;
	char Backup[255];

	m = strlen(s);
	for(k=1; k<strlen(s);k++)
	{
		MatchJump[k] = 2*m - k;
	}
		while (k>0)
		{
			Backup[k]=q;
			while( (q <= m) && (s[k] != s[q]))
			{
			MatchJump[q] = MIN(MatchJump[q], m-k);
			q = Backup[q];
			}
		k = k-1;
		q = q-1;		
		}

	for(k = 1; k<q; k++)
	{
		MatchJump[k] = MIN(MatchJump[k], m+q-k);
	}

	qq = Backup[q];
	while(q<m)
	{
		while(q<qq)
		{
			MatchJump[q] = MIN(MatchJump[q], qq-q+m);
			q = q+1;

		}
		qq=Backup[qq];
	}
}

	void BMA(*p, *t)
	{
		printf("%s\n",p );
		
		char pole[255];
		char pole2[255];
		ComputeJumps("lol",pole[255]);
		ComputeMatchJump("lol",pole2[255]);


		int j,k;

		while(j<strlen(t) && (k>0))
		{
			if(t[j] == p[k])
			{
				j--;
				k--;
			}
			j = j+MAX(CharJumpP[t[j]], MatchJumpP[k]);
			k = strlen(&p);

		}
	
	if (k==0)
	{
		printf("%s\n","shoda" );
	}
	else
	{
		//BMA = strlen(t)+1;
		printf("%s\n","neshoda" );
	}
	//}
}


int main() {
    //testovaci retezec
	BMA("lol","ololol");

}
