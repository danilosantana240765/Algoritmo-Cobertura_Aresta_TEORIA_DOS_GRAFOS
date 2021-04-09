# include <stdio.h>
# include <stdlib.h>
# include <locale.h>
# include <string.h>

/*
* Funcao pra verificar se o arquivo existe 
*/
int isFile(char * arquivo){
	return !(fopen(arquivo, "r") == NULL);
}

/*
* Funcao para tirar o "\n" do final do arquivo 
*/
void retiraQuebraLinha(char * string){
	if(string[strlen(string) - 1] == '\n')
		string[strlen(string) - 1] = '\0'; 
}

/*
* Funcao para mostrar a matriz de adjacente 
*/
void mostrarMatrizAdj(int ** matrAdj, int N){
	int i, j;
	printf("~~~~~~~~~~~~~~ MATRIZ DE ADJACENTE ~~~~~~~~~~~~~~~~ \n");
	for(i = 0; i < N; i++){
		printf("\t");
		for(j = 0; j < N; j++){
			printf("%3d", matrAdj[i][j]);
			fflush(stdin);
		}
		printf("\n");
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/*
* Funçao para liberar a mameria alocar em uma matriz 
*/
void liberarMemoria(int **matr, int N){ 
	int pos;
	for(pos = 0; pos < N; pos++){
		free(matr[pos]); // Liberar as linhas
	}
}

/*
* Funcao para ler a quantidade de linha do arquivo
*/
int getTotLinhaArquivo(char * arquivo){
	int tot = 0;
	if(isFile(arquivo)){
		FILE *arq = fopen(arquivo, "r");
		char leitura[1000];
		
		// Realizada a contagem de linha que tem o arquivo a ser lido 
		while(fgets(leitura, sizeof(leitura), arq) != NULL){
			retiraQuebraLinha(leitura);
			if(strlen(leitura) > 0)
				tot++;
		}
		
		fclose(arq);
	}
	
	return tot;
}

/*
* Funcao para ler o arquivo e jogar na matriz de adjacencia  
*/
int ** lerArquivo(char * arquivo, int * N){ 
	int ** matrAdj = NULL; // A matriz que guardará os valores da matriz de adjacênte
	char leitura[1000]; // Variavel para receber as string do arquivo que vai auxiliar na funcao gets() para ler arquivo
	int totLinhaAnterior; // Variável para auxiliar na validação da matriz
	int totLinha = 0; // Variavel para contar as linhas
	int totColuna; // Variavel para colocar os valores na matriz corretamente, será usado para auxiliar no strtok()
	char *aux; // Para ajudar a auxiliar na quebra das string na funcao strtok()
	
	// Alocar a linha da matriz com a quantidade de linha que tem o arquivo lido
	matrAdj = (int**) malloc(sizeof(int*) * getTotLinhaArquivo(arquivo));
			
	// Verificar a matriz alocar
	if(matrAdj == NULL){
		printf("<<< ERRO! Nao foi possivel alocar a matriz %d ! >>>\n", totLinha);
		exit(100); 
	}
	
	// Verificar se o arquivo existe
	if(isFile(arquivo)){
		
		// Abrindo o arquivo
		FILE *file = fopen(arquivo, "r");
		
		// Ler as linhas do arquivo
		while(fgets(leitura, sizeof(leitura), file) != NULL && file != NULL){
			// Retirando o "\n" da leitura do arquivo caso exista
			retiraQuebraLinha(leitura);
			
			// Alocar as colunas
			matrAdj[totLinha] = (int*) malloc(sizeof(int) * strlen(leitura));
			
			// Verificar se a coluna foi alocada
			if(matrAdj[totLinha] == NULL){
				printf("<<< ERRO! Nao foi possivel alocar a matriz! >>>\n");
				exit(200); 
			}
			
			/*
			* Seperando as string por unidade, ou seja vamos quebrar pelos espaço
			* a string. Exemplo: 1 0 0 1: quebra pelo espaço e pegamos somente os valores
			* ficaria {1, 0, 0, 1} sem os espaços
			*/
			aux = strtok(leitura, " ");
			totColuna = 0; 
			while(aux != NULL){ 
				matrAdj[totLinha][totColuna] = atoi(aux);
				aux = strtok(NULL, " ");
				totColuna++;
			}
			
			// Verificar se a linha anterior é igual a atual, se for é valida, senão é invalida
			if(totLinha == 0){
				totLinhaAnterior = totColuna;
			}else {
				if(totColuna != totLinhaAnterior){
					// chamar a funcao para desalocar memoria
					liberarMemoria(matrAdj, totLinha); 
					return NULL;
				}
				totColuna = totColuna;
			}
			
			// Quantidade de linha lida do arquivo
			totLinha++; 
			
		} // Fim do While
		
		// Fechando o arquivo
		fclose(file); 
	}
	
	// Guarda a dimensão da matriz de adjacente
	*N = totLinha;
	
	// Retornando a matriz
	return matrAdj;
}

/*
* Funcao para pesquisar se dentro da matriz já existe as coordenada salva
* basicamente ele irá retorna 1 para verdadeiro e 0 para falso
* O(N)
*/
int buscaArestaAdjacente(int **matr, int N, int i_busca, int j_busca){
	int i = 0;
	int encontrado = 0; 
	
	/* Buscando por vertice que são adjacente */
	while(i < N && !encontrado){     
		if(matr[0][i] == i_busca || matr[1][i] == j_busca  || matr[0][i] == j_busca || matr[1][i] == i_busca){
			encontrado = 1;
		}
		i++;
	}
	
	return encontrado; 
}

/*
* Funcao pra selecionar os vertice não fizeram parte dos vertices pares não adjacente.
* 
*/
void capturarVertices(int **matr_fixacao, int **matr_adj, int N, int N_fixo, int * tot_fixacao){
	int j = 0;
	int isLaco = 0;
	if(N >= 0){
		// Verifica se o vertice que ficou de fora, não foi selecionado no momento que faz a seleção de pares de vertices
		if(!buscaArestaAdjacente(matr_fixacao, *tot_fixacao, N, N)){
			/* Esse loop busca por uma aresta, pelo busca por uma aresta se caso
			 * não tiver aresta, busca pelos laço
			 */
			while((matr_adj[N][j] == 0 || N == j) && j < N_fixo){ 
				// Verificando se tem laço
				if(matr_adj[N][j] != 0 && N == j){
					isLaco = 1;
				}
				j++;
			}
			
			// Selecionando uma aresta sem ser laço, caso o vertice tenha aresta sem contar com os laço
			if(j < N_fixo && matr_adj[N][j] != 0){
				matr_fixacao[0][*tot_fixacao] = N;
				matr_fixacao[1][*tot_fixacao] = j;
				(*tot_fixacao)++;
			}else if(isLaco){ // Caso esse vertice só tenha laço.
				matr_fixacao[0][*tot_fixacao] = N;
				matr_fixacao[1][*tot_fixacao] = N;
				(*tot_fixacao)++;
			}
		
		}
		
		// Chamada recursiva para verificar se todos os vertice não esta no conjunto 
		capturarVertices(matr_fixacao, matr_adj, N - 1, N_fixo, tot_fixacao);
	}
}


/* Criando uma funcao para verificar se o grafo é um conjuto independete de vertices  */
int isConjuntosIndependeteVertice(int **matr_adj, int N){
	int i = 0;
	int j = 0;
	int isAresta = 0;
	while(i < N && !isAresta){
		while(j < N && !isAresta){
			// Verificando se tem alguma aresta
			if(matr_adj[i][j] != 0){
				isAresta = 1;
			}
			j++;
		}
		i++;
	}
	
	return !isAresta;
}

/* Funcao para verificar se todos os vertice tem aresta, caso algum vertice seja desconexo sem vertice, entao retorne falso */
int isCoberturaAresta(int **matr, int N, int N_fixo){
	int i;
	int isvalido;
	
	if(N > 0){
		isvalido = 0;
		i = 0;
		while(i < N_fixo && !isvalido){
			isvalido = matr[N - 1][i] != 0;
			i++;
		}
		
		// Nessa expressão verifica a expressao lógica váriável isvalido com o retorno da chamada recursiva
		return isvalido && isCoberturaAresta(matr, N -1, N_fixo);
	}
	
	return 1;
}

/* Criando uma funcao para verificar se o grafo so tem laço */
int isGrafoSoLaco(int **matr_adj, int N){
	int i = 0;
	int j = 0;
	int isInvalido = 0; 
	while(i < N && !isInvalido && !isConjuntosIndependeteVertice(matr_adj, N)){
		while(j < N && !isInvalido){
			if(matr_adj[i][j] != 0 && i != j){
				isInvalido = 1;
			}
			j++;
		}
		i++;
	}
	
	return !isInvalido;
}

/* 
*  Objetivo dessa função é selecionar as aresta que não são adjacente e guardar dentro de uma matriz 
*  MATR[2][N] que será o conjunto de aresta que não sao adjacente
* 
*/
int ** getVerticeNaoAdjacente(int ** matr, int N, int i_fixado, int j_fixado, int * tot_cobertura){
	int i, j;
	/* 
	* Matriz para guardas os pares de vertices que não tocam. Basciamente essa  matriz é [2][N]
	* onde irá guardar pares de arestas que são cobertura. 
	* Vamos supor que a aresta A(1,2 ). Então o conjunto será guardado:
	* matriz_de_fixacao[0][N] = 1, onde somente N irá váriar
	* matriz_de_fixacao[1][N] = 2, onde somente N váriar
	* Quando o vertice tem A(x, y) o armazenamento será:
	* A linha 0 e coluna  N da matriz guarda o vertice x 
	* A linha 1 e coluna N da matriz guarda o vertice y
	*/
	int ** matriz_de_fixacao = (int**) malloc(sizeof(int*) * 2);
	
	// Alocando as linha
	if(matriz_de_fixacao == NULL){
		printf("<<< ERRO! Nao foi possivel alocar espaco da matriz de fixacao >>>\n");
		exit(300);
	}
	
	/* Alocando as colunas */ 
	for(i = 0; i < 2; i++){
		matriz_de_fixacao[i] = (int*) malloc(sizeof(int) * N);
	}
	
	int tot = 0; // Variavel para usar na "matriz_de_fixacao" para guardar o conjutos de aresta que tocam em todos os vertices
	
	/*
	* O objetivo desse comando é basicamente verificar se o valor da matriz é diferente de zero para evitar fixar valores inválido
	* e depois as arestas que nao sao adjacente será guardado dentro da matriz chamada "matriz_de_fixacao", onde guardará o par
	* de vertice que é a aresta que faz cobertura
	* -- Primeiro verificacão: se a matriz adjacente na posicao [i_fixo][j_fixo] não seja igual a zero e também 
	* 	não seja laços, para evitar o algoritmo de jogar laço como cobertura de aresta.
	* -- Segunda verificação: se nesse grafo só tem laços, isso porque for grafos que só não tem nenhuma aresta e nenhum vertice
	*	e esse vertices tenha algum laço, pegar esse laço como cobertura 
	*/
	if(matr[i_fixado][j_fixado] != 0 && i_fixado != j_fixado){ // -- Primeira verificação
		
		// Jogando os primeiro pares de vertices que são cobertura de aresta na primeira posição
		matriz_de_fixacao[0][tot] = i_fixado;
		matriz_de_fixacao[1][tot] = j_fixado;
		tot++;
		
		// Fazendo uma repetição para verificar outros pares de vertice que pode ser formado para ser cobertura
		for(i = 0; i < N; i++){ 
			for(j = 0; j < N; j++){
				/*
				* Basicamente para fazer a cobertura de aresta, primeiro busca na "matriz_de_fixacao()" se
				* dentro dela não existe nenhum vertice que seja adjecente, caso já seja, entao não entra na condição
				*/
				if(matr[i][j] != 0 && i != j){
					if(!buscaArestaAdjacente(matriz_de_fixacao, tot, i, j)){
						matriz_de_fixacao[0][tot] = i;
						matriz_de_fixacao[1][tot] = j;
						tot++;
					}
				}
					
			}
		}
		
		/* Chamar a funcao para colocar um vertice que ficou de fora da cobertura de aresta
		* Se por exemplo tiver 5 vertices, e vamos supor que desse 5 vertice somente as aresta
		* A(1, 2), A(3, 4) é cobertura, só que ficou faltando o vertice 5, devido o vertice 5 não formar par com outro
		* vertice para ter uma aresta que seja cobertura, esse vertice ficou de fora, masi colocar ele também no conjunto
		* é usada essa funcao "capturarVertices()" onde basicamente verifica quais vertices ficou de fora do conjuto
		* aquele que ficou de fora, entao é colocado no conjuto, mesmo essa tendo adjacencia com outro vertice.
		*/
		capturarVertices(matriz_de_fixacao, matr, N - 1, N, &tot);
	}else if(isGrafoSoLaco(matr, N)){ // -- Segunda verificação 
		capturarVertices(matriz_de_fixacao, matr, N - 1, N, &tot);
	}else {
		liberarMemoria(matriz_de_fixacao, 2);
		matriz_de_fixacao = NULL;
	}
	
	*tot_cobertura = tot; // Total de aresta selecionado
	return matriz_de_fixacao;
}

/* Funcao para verificar se o grafo é valorado  */
int isGrafoValorado(int **matr, int N){
	int i, j;
	
	/* Basicamente é verificar se todos os bits 1 são igual ao total, caso não seja, é porque é valorado */
	int totalBit1 = 0;
	int total = 0;
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(matr[i][j] == 1){
				totalBit1++;
			}
			
			if(matr[i][j] != 0){
				total ++;
			}
		}
	}
	
	return !(totalBit1 == total);
}

/* Funcao para realizar a cobertura de aresta */
void cobertura_aresta(int ** matr, int N){
	
	int i, j;
	int ** conj_cobertura_minimal = NULL; // Variavel que vai guardar a menor cobertura de aresta que seja minimal
	int tot_cobertura_minimal; // Variavel que diz a quantidade da cobertura minimal de aresta. Quantidade de aresta
	int ** conj_cobertura;
	int ** aux;
	int tot_cobertura;
	
	/*
	* O objetivo desse "for" é basicamente pegar cada valor da matriz de adjacencia e comparar com todos os
	* valores da mesma matriz, onde irá verificar se é adjacente.
	*/
	
	if(isConjuntosIndependeteVertice(matr, N) || !isCoberturaAresta(matr, N, N)){ 
		printf("*** Nao tem cobertura! ***\n");
	}else { 
		for(i = 0; i < N; i++){ 
			for(j = 0; j < N; j++){ 
			
				// Recebe o retorno da funcao
 				conj_cobertura = getVerticeNaoAdjacente(matr, N, i, j, &tot_cobertura); 
				
				/*
				* Basicamente aqui pegar a menor conjunto de aresta que compoe a combertura de aresta
				* caso o conj_cobertura_minimal for nula é porque não tem nenhuma ainda no conjunto, 
				* entao ja recebe a primeiro conjunto de depois compara que é o maior e quem é o menor
				* caso o conjunto seja menor do que retorna da funcao getVerticeNaoAdjacente() entao o
				* conj_cobertura_minimal recebe esse menor
				*/
				if(conj_cobertura != NULL && (conj_cobertura_minimal == NULL || tot_cobertura < tot_cobertura_minimal )){
					aux = conj_cobertura_minimal;
					conj_cobertura_minimal = conj_cobertura;
					tot_cobertura_minimal = tot_cobertura;
					// Caso aux tenha algum endereço de memoria, liberar memoria
					if(aux != NULL){
						liberarMemoria(aux, 2); 
					}
				}
			
				
			}
		}
		
		// Mostrando a cobertura minimal
		printf("COBERTURA MINIMAL: {");
		
		// Caso seja um grafo pesado ou valorado, mostrar os pesos
		if(isGrafoValorado(matr, N)){
			for(int i = 0; i < tot_cobertura_minimal; i++){
				printf("%d", matr[conj_cobertura_minimal[0][i]][conj_cobertura_minimal[1][i]]);
				if(i < tot_cobertura_minimal - 1){
					printf(", ");
				}
			}
		}else { // Aqui mostra os valores rotulado no formato de A(x, y) 
			for(int i = 0; i < tot_cobertura_minimal; i++){ 
				printf("A(%d, %d)", conj_cobertura_minimal[0][i] + 1, conj_cobertura_minimal[1][i] + 1);
				if(i < tot_cobertura_minimal - 1){
					printf(", ");
				}
			}
		}
		
		printf("}");
	}
	
	printf("\n-------------------------------------\n\n\n");
}

/* Funcao pra transformar uma letra minusculas para maiscula
* Obs: Agente criou essa funcao, porque a função nativa da linguagem C++ ou C não estava funcionando
*/
char * caixaAlta(char * string){
	int pos;
	char * new_string = (char*) malloc(sizeof(char));
	strcpy(new_string, string);
	
	for(pos = 0; pos < strlen(string); pos++){
		if(new_string[pos] >= 97 && new_string[pos] <= 122){
			new_string[pos] = new_string[pos] - 32;
		}
	}
	
	return new_string;
}

int main(){
	setlocale(LC_ALL, "Portuguese");
	
	char * arquivo = (char*) malloc(sizeof(char));
	char * flag; // Variavel que vai guardar o endereço do flag de saida
	int ** matrAdj;
	int N;
	int pos;
	
	printf("Ola, vamos comecar...\n");
	/* Leitura de arquivo */
	do {
		printf("Digite o nome do arquivo: (SAIR sai do programa) ");
		gets(arquivo);
		fflush(stdin);
		// Comando do prompt do windows para limpar o terminal
		system("cls");  
		
		// Variavel para controlar a saida da repitição do while
		flag = caixaAlta(arquivo); 
		// Verificando se o arquivo existe
		if(strcmp(flag, "SAIR") != 0){
			// Concatena com a extensão
			strcat(arquivo, ".txt");
			// Verificando se o arquivo existe
			if(isFile(arquivo)){ 
				
				printf("ARQUIVO ABERTO: <%s>\n", arquivo);
				printf("----------------------------------\n");
				
				matrAdj = lerArquivo(arquivo, &N); // Chamando a funcao para ler o arquivo //
				
				// Verificar se a matriz foi lida
				if(matrAdj != NULL && N > 0){ 
					mostrarMatrizAdj(matrAdj, N);
					cobertura_aresta(matrAdj, N); 
				}else {
					printf("<<< ERRO! Matriz invalida! >>>\n");
				}
			}else {
				printf("<<< Arquivo \"%s\" nao existe! >>>\n", arquivo);
			}
	
		}else {
			printf("<<< FECHANDO o programa >>>\n");
		}

	}while(strcmp(flag, "SAIR") != 0);
	
	// Liberar memoria
	free(arquivo); 
	free(flag);
}
