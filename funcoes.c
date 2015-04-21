#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcoes.h"

//insere no fim da lista uma nova celula
void insereCelula(char rotulo[], int endereco, int dir, celula *ini)
{
	celula *p, *q, *nova;
	nova = malloc(sizeof(celula));
	strcpy(nova -> rotulo, rotulo);
	nova -> endereco = endereco;
	nova -> dir = dir;
	nova -> prox = NULL;
	p = ini;
	q = ini->prox;
	while (q != NULL) {
		p = q;
		q = q->prox;
	}
	p -> prox = nova;
}

//imprime celulas
void imprimeCelula(celula *p)
{	
	while(p){
		printf("Rotulo: %s - Endereco: %d - Dir: %d\n", p -> rotulo, p -> endereco, p -> dir);
		p = p -> prox;
	}
}

void imprimeErro(char *erro, char *dir, int linha){
	printf(erro, dir);
	printf(LINHA_ERRO, linha+1);
}

//procura valor nas celulas
celula procuraCelula(celula *p, char *rotulo)
{
	celula nula;
	nula.dir = 0;
	
	if(rotulo){
		while (p) {
			if(strcmp(p -> rotulo, rotulo) == 0)
				return *p;
			
			p = p -> prox;
		}
	}
	
	return nula;
}

/* transforma string em upper case */
void stringToUpper(char *str){
	int size = strlen(str);
	int i;
  
	for(i=0;i<size;i++)
		str[i] = toupper(str[i]);
}

/* Valida numero. Retorna 0 se nao for valido, 1 se for decimal, 2 se for hexadecimal, 
 * 3 se for binario, 4 se for octal */
int validaNumero(char *n){
	int i;
	int k = strlen(n);
	
	// hexadecimal
	if(n[0] == '0' && n[1] == 'X'){
		for(i = 2; i < k; i++)
			if(((n[i] < 48) || (n[i]>57)) && ((n[i] < 65) || (n[i] > 70)))
				return 0;
		return 2;
	}

	// binario
	else if((n[0] == '0') && (n[1] == 'B')){
		for(i = 2; i < k; i++)
			if((n[i] < 48) || (n[i] > 49))
				return 0;
		return 3;
	}
	
	// octal
	else if((n[0] == '0') && (n[1] == 'O')){
		for(i = 2; i < k; i++)
			if((n[i] < 48) || (n[i] > 55))
				return 0;
		return 4;
	}
	
	// decimal
	else{
		for(i = 2; i < k; i++)
		if((n[i] < 48) || (n[i] > 57))
			return 0;
		return 1;
	}

}

// converte de string para int o endereco
int converteEndereco(char *endereco){
	int resultado = validaNumero(endereco);
	unsigned int valor;
	int k, i;
	int base = 1;
	
	if(resultado){
		// decimal
		if(resultado == 1)
			valor = atoi(endereco);
			
		// binario
		else if(resultado == 3){
			valor = 0;
			k = strlen(endereco);
			
			for (i = k-1; i > 1; i--){
				if(endereco[i] == '1')
					valor = valor + base;
				base = base * 2;
			}
		}
		
		// hexadecimal
		else if(resultado == 2)
			sscanf(endereco,"%x",&valor);
			
		// octal
		else if(resultado == 4)
			sscanf(&endereco[2],"%o",&valor);
		
		return valor;
	}
	
	// invalido
	return 0;
}

int getEndereco(char *string, celula *rotulos, int j){
	celula resultado;
	char n[1000];
	char *aux;
	int i;
	int k;

	k = strlen(string);

	if((string[0] == 'M') && (string[1] == '(') && (string[k - 1] == ')')){
		//copia para n o endereco
		for(i = 2; i < k - 1; i++)
			n[i-2] = string[i];
		n[k - 3] = '\0';
		aux = strtok(n, ",");
		
		resultado = procuraCelula(rotulos, aux);
		
		//caso nao seja um rotulo, converte o numero
		if(resultado.dir == 0)
			resultado.endereco = converteEndereco(aux);
			
		return resultado.endereco;
	}
	
	else
		imprimeErro(ERRO_ARG_MEMORIA, string, j);
	
	return 0;
}

// checa se a instrucao e valida
int interpretaInstrucao(char *opcode, char *endereco, celula *rotulos, int linha){
	celula p;
	char n[1000];
	char *aux;
	int i, k;
	
	p.dir = 0;

	if(strcmp(opcode, "LOAD") == 0){
		return 1;
	}
	else if(strcmp(opcode, "LDMQ") == 0){
		return 10;
	}
	else if(strcmp(opcode, "LDMQM") == 0){
		return 9;
	}
	else if(strcmp(opcode, "LDN") == 0){
		return 2;
	}
	else if(strcmp(opcode, "LDABS") == 0){
		return 3;
	}
	else if(strcmp(opcode, "ADD") == 0){
		return 5;
	}
	else if(strcmp(opcode, "ADDABS") == 0){
		return 7;
	}
	else if(strcmp(opcode, "SUB") == 0){
		return 6;
	}
	else if(strcmp(opcode, "SUBABS") == 0){
		return 8;
	}
	else if(strcmp(opcode, "MUL") == 0){
		return 11;
	}
	else if(strcmp(opcode, "DIV") == 0){
		return 12;
	}
	else if(strcmp(opcode, "LSH") == 0){
		return 20;
	}
	else if(strcmp(opcode, "RSH") == 0){
		return 21;
	}
	else if(strcmp(opcode, "STR") == 0){
		return 33;
	}
	//instrucoes que dependem de pra onde o rotulo aponta
	else if(endereco){
		k = strlen(endereco);

		if((endereco[0] == 'M') && (endereco[1] == '(') && (endereco[k-1] == ')')){
			//copia para n o endereco
			for(i = 2; i < k - 1; i++)
				n[i-2] = endereco[i];

			n[k-3] = '\0';
			aux = strtok(n, ",");
			k = converteEndereco(aux);
			
			//o argumento é um rotulo
			if(k == 0){
				p = procuraCelula(rotulos, n);
			
				if(strcmp(opcode, "STM") == 0){
					if(p.dir == 1)
						return 19;
					else
						return 18;
				}
				
				else if(strcmp(opcode, "JMP") == 0){
					if(p.dir == 1)
					  return 14;
					else
					  return 13;
				}
				
				else if(strcmp(opcode,"JGEZ") == 0){
					if(p.dir == 1)
						return 16;
					else
						return 15;
				}
			}
			
			//o argumento é um numero
			else{
				aux = strtok(NULL, ",");
				
				if(aux == NULL){
					imprimeErro(ERRO_FALTA_BITS, opcode, linha);
					return 0;
				}
				
				if(strcmp(opcode, "STM") == 0){
					if(strcmp(aux, "28:39") == 0)
						return 19;
					else if(strcmp(aux, "8:19") == 0)
						return 18;
				}
				
				else if(strcmp(opcode, "JMP") == 0){
					if(strcmp(aux, "20:39") == 0)
					  return 14;
					else if(strcmp(aux, "0:19") == 0)
					  return 13;
				}
				
				else if(strcmp(opcode,"JGEZ") == 0){
					if(strcmp(aux, "20:39") == 0)
						return 16;
					else if(strcmp(aux, "0:19") == 0)
						return 15;
				}
				
				imprimeErro(ERRO_FALTA_BITS, opcode, linha);
				return 0;
			}
		}
	}
	
	return 0;
}

// checa se a instrucao e valida
int checaInstrucao(char *opcode){

	if(strcmp(opcode, "LOAD") == 0)
		return 1;
		
	else if(strcmp(opcode, "LDMQ") == 0)
		return 2;
		
	else if(strcmp(opcode, "LDMQM") == 0)
		return 1;
		
	else if(strcmp(opcode, "LDN") == 0)
		return 1;
		
	else if(strcmp(opcode, "LDABS") == 0)
		return 1;
		
	else if(strcmp(opcode, "ADD") == 0)
		return 1;
		
	else if(strcmp(opcode, "ADDABS") == 0)
		return 1;
		
	else if(strcmp(opcode, "SUB") == 0)
		return 1;
		
	else if(strcmp(opcode, "SUBABS") == 0)
		return 1;
		
	else if(strcmp(opcode, "MUL") == 0)
		return 1;
		
	else if(strcmp(opcode, "DIV") == 0)
		return 1;
		
	else if(strcmp(opcode, "LSH") == 0)
		return 2;
		
	else if(strcmp(opcode, "RSH") == 0)
		return 2;
		
	else if(strcmp(opcode, "STR") == 0)
		return 1;
		
	else if(strcmp(opcode, "STM") == 0)
		return 1;
			
	else if(strcmp(opcode, "JMP") == 0)
		return 1;
			
	else if(strcmp(opcode,"JGEZ") == 0)
		return 1;
	
	return 0;
}

//retorna o numero de linhas
int numberLines(char *file){
    int count=0;
    int i;
    
    for(i = 0; file[i] != '\0'; i++)
		if( file[i] == '\n') count++;
	
	return count;
}

//retorna o numero de palavras
int numberWords(char *line){
    int count = 0;
    int i = 0;
    
	while(line[i] != '\n' && line[i] != '#' && line[i] != '\0'){
		if(line[i] != ' '){
			count++;
			while(line[i] != ' ' && line[i] != '\n' && line[i] != '#' && line[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return count + 1;
}

//verifica o tipo da string.
char* typeString(char *string){
	int i;
	// diretiva
	if(string[0] && string[0] == '.'){
		if(string[1] && strcasecmp(&string[1], "ORG") == 0)
			return "org";
		else if(string[1] && strcasecmp(&string[1], "ALIGN") == 0)
			return "align";
		else if(string[1] && strcasecmp(&string[1], "WFILL") == 0)
			return "wfill";
		if(string[1] && strcasecmp(&string[1], "WORD") == 0)
			return "word";
		if(string[1] && strcasecmp(&string[1], "SET") == 0)
			return "set";
		else
			return "erro";
	}
	// instrucao
	i = checaInstrucao(string);
	if(i == 2)
		return "instrucao_sem_arg";
	else if(i == 1)
		return "instrucao";
		
	//rotulo
	char *fim = strrchr(string, ':');
	if (fim && !strcmp(fim, ":")){
		return "rotulo";
	}
	
	else
		return "outro";
}

//encontra os pares rotulos-enderecos
void firstFileRun(char ***text, celula *rotulos, celula *sets){
	int j, i, k; 
	char temp[500];
	char *tipo;
	celula atual, v1, v2;
	
	atual.endereco = 0;
	atual.dir = -1;
	
	//monta a lista de rotulos - enderecos
	for(j = 0; j < n_lines; j++){
		for(i = 0; text[j][i]; i++){
			tipo = typeString(text[j][i]);
			
			if(text[j][i+1]){
				v1 = procuraCelula(sets, text[j][i+1]);

				if(v1.dir == 0)
					v1.endereco = converteEndereco(text[j][i+1]);
			}
			
			if(text[j][i+2]){
				v2 = procuraCelula(sets, text[j][i+2]);
			
				if(v2.dir == 0)
					v2.endereco = converteEndereco(text[j][i+2]);
			}
			
			//trata diretiva set
			if(strcmp(tipo, "set") == 0){
				if(text[j][i+1] && text[j][i+2]){
					k = strlen(text[j][i+1]);
					strcpy(temp, text[j][i+1]);
					temp[k-1] = '\0';
					
					v1 = procuraCelula(sets, temp);
					
					if(v1.dir == 0)
						insereCelula(temp, v2.endereco, -1, sets);
					else
						imprimeErro(ERRO_SET_EXISTENTE, temp, j);
					
					i = i + 2;
				}
				else
					imprimeErro(ERRO_DIR_ARG, ".set", j);
			}
			
			//trata diretiva org
			else if(strcmp(tipo, "org") == 0){
				if(text[j][i+1]){
					atual.endereco = v1.endereco;
					atual.dir = -1;
					
					i++;
				}
				else
					imprimeErro(ERRO_DIR_ARG, ".org", j);
			}
			
			//trata diretiva align
			else if(strcmp(tipo, "align") == 0){
				if(text[j][i+1]){
					k = atual.endereco;
					
					if(atual.dir == 1)
						k++;
					
					while(!(k % v1.endereco == 0))
						k++;
					
					atual.endereco = k;
					atual.dir = -1;
					
					i++;
				}
				else
					imprimeErro(ERRO_DIR_ARG, ".align", j);
			}
			
			//trata diretiva word
			else if(strcmp(tipo, "word") == 0){
				if(atual.dir == -1){
					if(text[j][i+1]){
						//se for constante simbolica, substitui
						if(v1.dir != 0){
							sprintf(temp, "%d", v1.endereco);
							strcpy(text[j][i+1], temp);
						}
						atual.endereco++;
						atual.dir = -1;
						
						i++;
					}
					else
						imprimeErro(ERRO_DIR_ARG, ".word", j);
				}
				
				else
					imprimeErro(ERRO_ALINHAMENTO, ".word", j);
			}
			
			//trata diretiva wfill
			else if(strcmp(tipo, "wfill") == 0){
				if(atual.dir == -1){
					if(text[j][i+1] && text[i][i+2]){
						//tira a virgula do argumento1
						k = strlen(text[j][i+1]);
						strcpy(temp, text[j][i+1]);
						temp[k-1] = '\0';
						
						//verifica se é uma constante simbólica, se for substitui pelo seu valor
						v1 = procuraCelula(sets, temp);
						if(v1.dir != 0){
							sprintf(temp, "%d,", v1.endereco);
							strcpy(text[j][i+1], temp);
						}
						else
							v1.endereco = converteEndereco(temp);
						
						//se for constante simbólica, subtitui pelo seu valor
						if(v2.dir != 0){
							sprintf(temp, "%d", v2.endereco);
							strcpy(text[j][i+2], temp);
						}
						
						atual.endereco = atual.endereco + v1.endereco;
						
						i = i + 2;
					}
					else
						imprimeErro(ERRO_DIR_ARG, ".wfill", j);
				}
				
				else
					imprimeErro(ERRO_ALINHAMENTO, ".wfill", j);
			}
			
			//trata instrucao
			else if(strcmp(tipo, "instrucao") == 0){
				if(atual.dir == 1)
					atual.endereco++;
				
				atual.dir = atual.dir * (-1);
				i++;
			}
			
			else if(strcmp(tipo, "instrucao_sem_arg") == 0){
				if(atual.dir == 1)
					atual.endereco++;
				
				atual.dir = atual.dir * (-1);
			}
			
			
			//trata rotulo
			else if(strcmp(typeString(text[j][i]), "rotulo") == 0){
				k = strlen(text[j][i]);
				strcpy(temp, text[j][i]);
				temp[k-1] = '\0';
				
				v1 = procuraCelula(rotulos, temp);
				if((temp[0] < 48) || (temp[0] > 57)){
					v1 = procuraCelula(rotulos, temp);
					
					if(v1.dir == 0)
						insereCelula(temp, atual.endereco, atual.dir, rotulos);
					else
						imprimeErro(ERRO_ROTULO_EXISTENTE, temp, j);
				}
				else
					imprimeErro(ERRO_ROTULO_INVALIDO, temp, j); 
			}
			
			else{
				imprimeErro(ERRO_COMANDO, text[j][i], j);
				
				//pula prox token
				i++;
			}
		}
	}
}

//encontra os pares rotulos-enderecos
void secondFileRun(char ***text, celula *rotulos, celula *sets, char* filename, char* output){
	int j, i, k, y, n;
	char temp[500];
	char *tipo, aux[10];
	FILE *file;
	celula atual, v1, v2;
	
	atual.endereco = 0;
	atual.dir = -1;
	
	if(!output)
		output = strcat(filename,".hex");
		
	file = fopen(output, "w");
	
	//monta a lista de rotulos - enderecos
	for(j = 0; j < n_lines; j++){
		for(i = 0; text[j][i]; i++){
			tipo = typeString(text[j][i]);
			
			//pula os rotulos
			if(tipo != "rotulo"){
				if(text[j][i+1]){
					v1 = procuraCelula(rotulos, text[j][i+1]);

					if(v1.dir == 0)
						v1.endereco = converteEndereco(text[j][i+1]);
				}
				
				if(text[j][i+2]){
					v2 = procuraCelula(rotulos, text[j][i+2]);
				
					if(v2.dir == 0)
						v2.endereco = converteEndereco(text[j][i+2]);
				}
				
				//trata diretiva set
				if(strcmp(tipo, "set") == 0){
					if(text[j][i+1] && text[j][i+2])
						i = i + 2;
				}
				
				//trata diretiva org
				else if(strcmp(tipo, "org") == 0){
					if(text[j][i+1]){
						if(atual.dir == 1)
							fprintf(file, "00 000\n");
							
						atual.endereco = v1.endereco;
						atual.dir = -1;
						
						i++;
					}
				}
				
				//trata diretiva align
				else if(strcmp(tipo, "align") == 0){
					if(text[j][i+1]){
						if(atual.dir == 1){
							fprintf(file, "00 000\n");
							atual.endereco++;
							atual.dir = -1;
						}
						
						k = atual.endereco;
						
						while(!(k % v1.endereco == 0))
							k++;
						
						atual.endereco = k;
						atual.dir = -1;
						
						i++;
					}
					else
						printf(ERRO_DIR_ARG, ".align");
				}
				
				//trata diretiva word
				else if(strcmp(tipo, "word") == 0){
					if(text[j][i+1]){
						if(v1.dir == 0){
							//caso seja numero negativo
							if(text[j][i+1][0] == '-'){
								k = converteEndereco(&text[j][i+1][1]);
								k = k*(-1);
							}
							//caso contrario
							else
								k = converteEndereco(text[j][i+1]);
						
							//encontra a representacao complemento do numero
							if(k < 0)
								k = 1099511627775 + k;
						}
						else
							k = v1.endereco;
						
						//armazena em temp o numero k em hexa
						sprintf(temp, "%X", k);
						
						y = 0;
						
						//converte para uma string de 10 caracteres, preenchida com 0s
						for(k = strlen(temp) - 1; k > -1; k--){
							aux[9-y] = temp[k];
							y++;
						}
						
						for(k = 9 - strlen(temp); k > -1; k--){
							aux[k] = '0';
						}
						
						//imprime no arquivo caracter por caracter
						fprintf(file,"%03X %c%c %c%c%c %c%c %c%c%c\n",atual.endereco, aux[0], aux[1], aux[2], aux[3], aux[4], aux[5], aux[6], aux[7], aux[8], aux[9]);
						atual.endereco++;
						atual.dir = -1;
						
						i++;
					}		
				}
				
				//trata diretiva wfill
				else if(strcmp(tipo, "wfill") == 0){
					if(atual.dir == -1){
						//armazena em temp o numero do argumento1 sem a virgula
						k = strlen(text[j][i+1]);
						strcpy(temp, text[j][i+1]);
						temp[k-1] = '\0';
						
						v1.endereco = converteEndereco(temp);
						
						if(v2.dir == 0){
							//verifica se argumento2 é negativo e converte o numero
							if(text[j][i+2][0] == '-'){
								k = converteEndereco(&text[j][i+2][1]);
								k = k*(-1);
							}
							else
								k = converteEndereco(text[j][i+2]);
						
							//converte para representação de complemento
							if(k < 0)
								k = 1099511627775 + k;
						}
						else
							k = v2.endereco;
						
						printf("%d\n", k);
						
						//armazena em temp o numero k em hexa
						sprintf(temp, "%X", k);
						
						y = 0;
						
						//converte para uma string de 10 caracteres, preenchida com 0s
						for(k = strlen(temp) - 1; k > -1; k--){
							aux[9-y] = temp[k];
							y++;
						}
						
						for(k = 9 - strlen(temp); k > -1; k--){
							aux[k] = '0';
						}
						
						//seja t o primeiro argumento de wfill, imprime t vezes
						n = atual.endereco + v1.endereco;
						while(atual.endereco < n){
							fprintf(file,"%03X %c%c %c%c%c %c%c %c%c%c\n",atual.endereco, aux[0], aux[1], aux[2], aux[3], aux[4], aux[5], aux[6], aux[7], aux[8], aux[9]);
							atual.endereco++;
						}
						
						//pula os proxs 2 tokens
						i = i + 2;
					}
				}
				
				//trata instrucao
				else if(strcmp(tipo, "instrucao") == 0){
					if(atual.dir == 1){
						fprintf(file,"%02X %03X\n", interpretaInstrucao(text[j][i], text[j][i+1], rotulos, j), getEndereco(text[j][i+1], rotulos, j));
						atual.endereco++;
					}
					else{
						fprintf(file,"%03X %02X %03X ",atual.endereco, interpretaInstrucao(text[j][i], text[j][i+1], rotulos, j), getEndereco(text[j][i+1], rotulos, j));
					}
					
					
					atual.dir = atual.dir * (-1);
					
					i++;
				}
				
				else if(strcmp(tipo, "instrucao_sem_arg") == 0){
					if(atual.dir == 1){
						fprintf(file,"%02X %03X\n", interpretaInstrucao(text[j][i], text[j][i+1], rotulos, j), 0);
						atual.endereco++;
					}
					else{
						fprintf(file,"%03X %02X %03X ",atual.endereco, interpretaInstrucao(text[j][i], text[j][i+1], rotulos, j), 0);
					}
					
					atual.dir = atual.dir * (-1);
					
				}						
			}
		}
	}
	
	//completa ultima palavra
	if(atual.dir == 1)
		fprintf(file, "00 000\n");
	
	fclose(file);
}

//le o arquivo
void read_ASM_file(char *filename, char *output)
{ 
	FILE *fp;
	char file[FILE_SIZE];

	fp = fopen(filename,"r");
	if (fp == NULL)
		printf("Erro ao abrir arquivo %s \n",filename);
	
	//copia para o array o arquivo
	char c;
	int i = 0;
	while ( (c=fgetc(fp)) != EOF ){
		file[i] = c;
		i++;
	}
	file[i] = '\0';

    stringToUpper(file); //todas letras maiusculas
	
	n_lines = numberLines(file); //numero de linhas no arquivo
	int n_words; //numero de palavras na linha
	char **text[n_lines]; //estrutura com o texto todo do programa
	char **words; //vetor das palavras
	int j = 0, k = 0;
		
	while(file[j] != '\0') {
		n_words = numberWords(&file[j]);
		words = malloc((n_words+1)*sizeof(int*));
		
		//armazena as palavras por linha
		i = 0;
		
		while(file[j] != '\n' && file[j] != '\0') {
			//ignora comentarios
			if(file[j] == '#'){
				while(file[j] != '\n' && file[j] != '\0')
					j++;
			}
			else{
				if(file[j] == ' '){
					file[j] = '\0';
					j++;
				}
				else if(file[j] != ' ' && file[j] != '\t' && file[j] != '\n' && file[j] != '\0'){
					words[i] = &file[j];
					i++;
					
					//percorre toda palavra
					while(file[j] != ' ' && file[j] != '\t' file[j] != '\n' && file[j] != '\0')
						j++;
				}
			}
		}
		
		if(file[j] == '\n'){
			file[j] = '\0';
			j++;
		}
		
		//marca final do vetor
		words[i+1] = NULL;
		
		text[k] = words;
		k++;
	}
	
	//realiza primeira leitura do programa para juntar os pares rotulos - enderecos. Tambem aponta os erros de sintaxe
	celula sets, rotulos;
	
	firstFileRun(text, &rotulos, &sets);
	
	//realiza segunda leitura e imprime o arquivo montado
	
	secondFileRun(text, &rotulos, &sets, filename, output);

	fclose(fp);
}
