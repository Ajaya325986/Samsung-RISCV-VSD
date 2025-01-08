#include <stdio.h>
int main(){
	int i, sum=0, n=100;
	for (i=0;i<n+1;++i){
	sum = sum + i;
	}
	printf("Sum of numbers from 1 to %d is %d\n",n,sum);
	return 0;
}
