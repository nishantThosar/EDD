/*Calc prog with func ptr*/

#include <stdio.h>
#include <stdlib.h>
int add(int,int);
int sub(int,int);
int mul(int,int);
int div_(int,int);


int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		perror("\nError Enter the values of a & b properly");
	}//
	if (argc > 3)
	{
		perror("\nMore than 3 arguments");
	}

	int (*func_ptr[])(int a,int b) = {add,sub,mul,div_}; 
	int uip;
	int ans;
	int a = atoi(argv[1]);if (a == 0){perror("Error zero inserted");exit(0);}
    int b  = atoi(argv[2]);if (b == 0){perror("Error zero inserted");exit(0);}

	printf("\nMenu\n1.Add\t2.Sub\t3.Mul\t4.Div");
	printf("\nEnter the option: ");
	scanf("%d" ,&uip);
	// printf("Enter value of a: ");
	// scanf("%d", &a);
	// printf("Enter value of b: ");
	// scanf("%d", &b);
	switch(uip)
	{
	case 1: //ans = add(a,b);
			ans = (func_ptr[0])(a,b);
		printf("Ans: %d", ans);
		break;
	case 2: ans =(func_ptr[1])(a,b);//sub(a,b);
				printf("Ans: %d", ans);
		break;	
	case 3: ans = (func_ptr[2])(a,b);//mul(a,b);
				printf("Ans: %d", ans);
		break;
	case 4: ans = (func_ptr[3])(a,b);//div_(a,b);
				printf("Ans: %d", ans);
		break;
	default: printf("Error wrong input");
	}//switch ends

	return 0;
}//main ends

int add(int a,int b)
{
	return a+b;
}//add ends

int sub(int a,int b)
{
	return a-b;
}//sub ends

int div_(int a,int b)
{
	return a/b;
}

int mul(int a,int b)
{
	return a*b;
}