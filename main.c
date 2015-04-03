#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main(int argc,char **argv)
{ 
	int n;
 
	n = atoi(argv[1]);
		
	if (n == 0) 
		read_ASM_file(argv[1]);
	else
		printf(ERRO_ARQUIVO);

	return(1);
}
