#define FILE_SIZE 8001
#define ERRO_ALINHAMENTO "ERRO: diretiva %s deve estar alinhada a esquerda. Tente usar a diretiva .align antes.\n"
#define ERRO_FIM_MEMORIA "ERRO: o programa a ser montado ultrapassa o endereco 1024 de memoria.\n"
#define ERRO_ARG_MEMORIA "ERRO: \"%s\" nao é um argumento válido para a instrução.\n"
#define ERRO_COMANDO "ERRO: \"%s\" nao é um comando válido.\n"
#define ERRO_DIR_ARG "ERRO: diretiva %s não recebeu argumentos suficientes.\n"
#define ERRO_ARQUIVO "ERRO: arquivo não encontrado.\n"
#define ERRO_ROTULO_INVALIDO "ERRO: rotulo \"%s\" é inválido.\n"
#define ERRO_SET_EXISTENTE "ERRO: constante simbolica \"%s\" ja existe.\n"
#define ERRO_ROTULO_EXISTENTE "ERRO: rotulo \"%s\" já existe.\n"
#define LINHA_ERRO "Linha: %d\n"

//estrutura de dados de lista ligada para os pares rotulo-endereco
struct cel {
	char rotulo[1000];
	int endereco;
	int dir;
    struct cel *prox;
};
typedef struct cel celula;

int n_lines;

void insereCelula(char rotulo[], int endereco, int dir, celula *ini);

void imprimeCelula(celula *p);

celula procuraCelula(celula *p, char *rotulo);

void stringToUpper(char *str);

int validaNumero(char *n);

int converteEndereco(char *endereco);

int getEndereco(char *string, celula *rotulos, int j);

int interpretaInstrucao(char *opcode, char *endereco, celula *rotulos);

int checaInstrucao(char *opcode);

int checaInstrucao(char *opcode);

int numberWords(char *line);

char* typeString(char *string);

void firstFileRun(char ***text, celula *rotulos, celula *sets);

void secondFileRun(char ***text, celula *rotulos, celula *sets, char* filename, char* output);

void read_ASM_file(char *filename, char* output);
