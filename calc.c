#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int i;
	
	double num1 = atof(argv[1]);
	double num2 = atof(argv[3]);
	char op = argv[2][0];
	double result;

	for(i = 0; i < argc; i++)
                printf("argv[%d]: %s \n", i, argv[i]);
	
	printf("\ncalc ");
	for(i = 0; i < argc; i++)
		printf("%s ",argv[i]);
	switch(op) {
		case '+':
			result = num1 + num2;
			break;
		 case '-':
                        result = num1 - num2;
                        break;
		case 'x':
                        result = num1 * num2;
                        break;
		 case '/':
                        result = num1 / num2;
                       break;
		}
	
	printf("\n%g\n", result);

	exit(0);
}
