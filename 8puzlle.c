#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>


typedef struct arvore{
	int cod;			//Codigo de cada nó
	int heuristica;		//Heuristica desse nó
	int matriz[3][3];	// Matriz desse nó
	struct arvore* rsup; // Ramificação com deslocamento para cima
	struct arvore* rinf; // Ramificação com deslocamento para baixo 
	struct arvore* rdir; // Ramificação com deslocamento para direita
	struct arvore* resq;  // Ramificação com deslocamento para esquerda
	char movimento[30]; // Texto com a jogada feita (EX: Movimento para direita)
	int qtdmov;	//Profundidade da arvore, ou seja, a quantidade de jogadas realizadas até ela
}Arvore;

int qtdmovimentos = 0;

typedef struct lista{
   int info;
   struct lista* prox;
}Lista;

Lista* L = NULL; // Lista que armazena as heuristicas dos nós que ainda não foram visitados.
Lista* L2 = NULL; // Lista que armazena o codigo dos nós que foram abertos.
Arvore* aux = NULL; // Aux que aponta para o nó em que a heuristica é a menor de toda a árvore
Arvore* aux2 = NULL; // Aux2 aponta para um no aberto para comparação de matrizes e evitar redundancias.

Lista* insereIni(int valor, Lista* L){ // Função que insere no incio de uma Lista
    Lista* aux = (Lista*) malloc(sizeof(Lista));
    aux->info = valor;
    aux->prox=L;
    return aux;
}

Lista* removeLista(Lista *L, int valor){ // Função que remove um valor de uma Lista
	
	Lista* aux = L;
	Lista* ant = NULL;
	
	while(aux->info!= valor && aux!=NULL){
		ant = aux;
		aux = aux->prox;
	}
	if(aux==NULL){
		return L;
	}
	if(ant == NULL){
		L= L->prox;
		free(aux);
	} else {
			ant->prox = aux->prox;
			free(aux);
	}
	
	return L;
}

int achaMenor(Lista* L) { // Função que acha a menor heurística de uma Lista
	if(L==NULL){
		return 10000; // Para nao afetar nas comparações, condição de parada
	}else{
		int menor;
		menor=achaMenor(L->prox); // chama recursivamente todos os valores da Lista
		if(menor<L->info) // compara quem é o menor, e retorna recursivamente para mais comparações
		return menor; // se for menor retorna ele
		else
		return L->info; 		// retorna o outro que estava sendo comparado
	}
}


// matriz global
int matriz[3][3]; // Matriz que irá carregar possibilidades de estado inicial
int matrizComp[3][3] = {{1,2,3},{4,5,6},{7,8,9}}; // Matriz com a estado final a qual irão ser comparados

//int matriz[3][3] = {{1,2,5},{6,8,7},{4,3,9}};
//int matriz[3][3] = {{1,2,3},{4,5,6},{7,9,8}};
//int matriz[3][3] = {{2,3,6},{1,4,9},{7,5,8}};
//int matriz[3][3] = {{2,9,5},{1,6,8},{4,3,7}};
//int matriz[3][3] = {{3,2,5},{6,1,8},{7,4,9}};
//int matriz[3][3] = {{4,3,2},{9,5,8},{1,6,7}};
//int matriz[3][3] = {{5,6,2},{4,9,8},{7,1,3}};

int codigo= 1; // codigo do nó, cada nó terá um codigo, toda vez que for criada, será incrementada

// verifica se existe valor na matriz, para fazer a matriz aleatoria 
int  existeValor(int valor){
		int i, j; 
		
		for(i = 0; i < 3; i++){
			for(j= 0; j < 3; j++){
				
				if(matriz[i][j] == valor){ // se existir esse valor 
					return 1; // retorna 1 dizendo que já existe
				}
			}
		}
		return 0; // retorna 0 dizendo que ainda não existe
}

void imprimeMatriz(int m[][3]){ // imprime qualquer matriz que for passada
	int i=0,j=0;
	
	for(i = 0; i < 3;i++){
		for(j = 0; j < 3; j++){
			if(m[i][j] == 9){
				printf("\t - "); // para melhor vizualizção e representa o valor nulo
			} else
				printf(" \t %d", m[i][j]);		// imprime o valor 
		}
			printf("\n");
	}
}


 criaMatriz(){ // Função que cria matriz aleatóriamente
	
	int num =0, i, j;
	int comp;
	
	
	
	for(i = 0; i < 3;i++){
		for(j= 0;j < 3; j++){
				 srand(time(NULL));
				 num = rand() % 9; // recebe um numero aleatorio
				 
				 while((existeValor(num)) == 1 || num == 0){ // chama a função e verifica se na matriz aleatoria ja existe esse numero
				 											 // não irá sair do laço enquanto nao ter um numero que nao esteja na matriz
				 		num = rand() % 10; // sorteia um numero
				 }
				
				matriz[i][j] = num; // matriz recebe esse numero
		}
	} 
	
	imprimeMatriz(matriz); // chama a funçao que imprime a matriz que foi criada acima
	
}

int calculaHeuristica(int m[][3]){//Função que calcula a heuristica através do metodo Manhattan Distance	
	int i=0, j = 0;
	int h=0,valor =0, linha = 0, coluna = 0,linhaComp =0, colunaComp=0; // guarda o valor da heuristica, e das quantidades de movimento em linha e coluna
	int flag=0; // auxiliar para identificar que achou a distancia desse valor para o seu estado final
	int cont=0; // conta quantos movimetos são necessarios até o valor chegar no seu estado final
	
	
	for(valor = 1; valor<=8; valor++){// encontrar a distancia de cada valor
		
		
			for(i = 0; i < 3;i++){
				for(j = 0; j < 3; j++){
					if(m[i][j] == valor ){
						linha = i;
						coluna = j;		
											// Encontra a linha e a coluna desse valor(matriz aleatoria)		
					}
				}
			}
		
			for(i = 0; i < 3;i++){
				for(j = 0; j < 3; j++){
					if(matrizComp[i][j] == valor ){	
						linhaComp = i;						// Encontra a posição em que esse valor deveria estar(matriz certa)
						colunaComp = j;
					}
				}
			}
			
		while(flag != 1 ){ // caso não achou a distancia desse valor, faça o laço
			
			if(linhaComp > linha){ // compara se a posição final da linha é maior do que a posição em que ela esta
				linhaComp--; // se for, a linhaComp diminui e fará ate os dois serem iguais.
				cont++; // incrementa o contador para cada vez que seria necessario fazer um movimento
			}
			
			else if(linhaComp < linha){ // compara se a posição final da linha é menor do que a posição em que ela esta
				linhaComp++; // se for, a linhaComp aumenta e fará ate os dois serem iguais.
				cont++; // incrementa o contador para cada vez que seria necessario fazer um movimento
			}
			
			else if(colunaComp > coluna){ // compara se a posição final da coluna é maior do que a posição em que ela esta
				colunaComp--; // se for, a colunaComp diminui e fará ate os dois serem iguais.
				cont++; // incrementa o contador para cada vez que seria necessario fazer um movimento
			}
			
			else if(colunaComp < coluna){ // compara se a posição final da coluna é menor do que a posição em que ela esta
				colunaComp++; // se for, a colunaComp aumenta e fará ate os dois serem iguais.
				cont++; // incrementa o contador para cada vez que seria necessario fazer um movimento
				
			}
			
			else{
				flag = 1; // caso seja tudo igual, quer dizer que achou a distancia desse valor  e sai do laço
				
			}
			
		}
		
		h = h + cont; // a heuristica recebe o valor que ela tem mais o contador dessa distancia
		cont = 0; // reseta o contador
		flag = 0; // aux recebe 0
			
	}
		return h; // apos achar a soma das distancias, retorna a heuristica
}



Arvore* insereArvore(int m[][3],int altura){ // cria um no da arvore
	
	int i=0;
	int j=0;
	
	Arvore* arv = (Arvore*) malloc(sizeof(Arvore)); // aloca a arvore
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			
			arv->matriz[i][j] = m[i][j]; // recebe a matriz correspondente a ela
			arv->rsup = NULL; 
			arv->rinf = NULL;
			arv->rdir = NULL;				// suas ramificações no momento recebem NULL, quando for abrir, outra função passa as ramificações
			arv->resq = NULL;
			arv->cod = codigo; // recebe o seu proprio codigo
			codigo++; // incrementa o codigo
		}
	}
	
	arv->heuristica = calculaHeuristica(arv->matriz); // calcula a heuristica e passa para esse no
	
	return arv; // retorna o no da arvore criada
	
}


void copia(int b[][3],int a[][3]) // função que recebe duas matrizes, e copia uma em outra
{

     int i,j;
     for(i=0;i<3;i++){
	 	for(j=0;j<3;j++){
			a[i][j]=b[i][j]; // a matriz a, recebe o valor da matriz b
  		}
	}

}

void troca(int *a,int *b) // recebe um ponteiro com o endereço do valor a ser trocado. Função para realizar os deslocamentos da matriz
{

     int aux;
     aux=*a;
     *a=*b;
     *b=aux;
								// ocorre a troca entre dois valores
}

void achaMatriz(Arvore* arv,int codigo){ // Função que acha a matriz pelo codigo dela
	
	if(arv != NULL){ // verifica se ela não é nula
	
	
		if(arv->cod != codigo ){ // procurará recursivamente o no correspondente, enquanto o codigo dela for diferente do codigo passado.
			achaMatriz(arv->rdir,codigo);
			achaMatriz(arv->resq,codigo);
			achaMatriz(arv->rsup,codigo);
			achaMatriz(arv->rinf,codigo);
		}
		
		else{
							// ao achar o codigo, o aux2 aponta para ela;
			aux2 = arv;
		}
	
	}
}

int compara(int matriz[][3],Arvore* raiz){// para evitar redundancias(compara se já teve  uma matriz aberta igual a  que será expandida)
	
	int i,j;
	int flag=0; // aux que retorna 1 caso não exista matriz igual
	
	Lista* Laux = NULL; // auxiliar para percorrer a lista
	Laux = L2;
	
	while(Laux != NULL){ // percorre toda a lista de abertos
		
		achaMatriz(raiz,Laux->info); // chama a função de achar a matriz pelo codigo, e o aux2 aponta para ela.

		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				
				if(matriz[i][j] != aux2->matriz[i][j]){ // caso todos os valores forem iguais, entao flag continua sendo 0
					flag = 1; // se tiver algum numero diferente, flag passa a ser 1
				}
			}	
		}
		
		if(flag == 0){
			return 1; // possui uma matriz aberta identica
			
		}
		
		else{
			flag = 0; // zera a flag
			Laux = Laux->prox; // aponta para o proximo da lista
		}
		
	}
	
	return 0; // não possui matriz desse tipo aberto
}

void arranjaArvore(Arvore* arv,Arvore* raiz){ // Função que vai abrir as possibilidades que o nó passado pode ter
	
	int i=0;
	int j = 0;
	
	//Aloca 4 nos que serão as possibilidades
	Arvore* rsup = (Arvore*) malloc(sizeof(Arvore));
	Arvore* rinf = (Arvore*) malloc(sizeof(Arvore));
	Arvore* rdir = (Arvore*) malloc(sizeof(Arvore));
	Arvore* resq = (Arvore*) malloc(sizeof(Arvore));
	
	// os nos alocados recebem o valor NULL para suas ramificações
	rsup->rdir = NULL;
	rsup->resq = NULL;	
	rsup->rsup = NULL;
	rsup->rinf = NULL;
	
	rinf->rdir = NULL;
	rinf->resq = NULL;
	rinf->rsup = NULL;
	rinf->rinf = NULL;
	
	resq->rdir = NULL;
	resq->resq = NULL;
	resq->rsup = NULL;
	resq->rinf = NULL;
	
	rdir->rdir = NULL;
	rdir->resq = NULL;
	rdir->rsup = NULL;
	rdir->rinf = NULL;
	
	// atribui a profundidade desses nós, sempre incrementando... (a profundidade é a quantidade de jogadas até chegar aquele estado)
	rsup->qtdmov = arv->qtdmov+1;
	rinf->qtdmov = arv->qtdmov+1;
	rdir->qtdmov = arv->qtdmov+1;
	resq->qtdmov = arv->qtdmov+1;
	
	int x,y; // variavel para pegar as coordenadas do valor nulo
	
	
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
			
				if(arv->matriz[i][j] == 9){ // acha o valor nulo e passa a linha e coluna do valor nulo
					x = i; 
					y=j; 
				}
			
			}
		}
	
	
	if((x-1) >=0 ){ // se ao diminuir o valor de x , ele for maior ou igual que 0, quer dizer que esta em um local que pode fazer esse movimento
		copia(arv->matriz, rsup->matriz);// copia uma matriz em outra
		troca(&rsup->matriz[x][y],&rsup->matriz[x-1][y]); // troca os valores
		rsup->heuristica = calculaHeuristica(rsup->matriz); // calcula a heuristica dessa matriz
		rsup->cod = codigo; // atribui um codigo a ela
		codigo++; // incrementa o codigo
		strcpy(rsup->movimento, "MOVER PARA BAIXO"); // mostra o movimento feito
				
	} else {//se ao diminuir o valor de x, ele for menor que 0, quer dizer que esta em um local que nao pode fazer esse movimento, entao recebe NULL
		rsup = NULL;
	}
	
	if((x+1) <= 2){// se ao aumentar o valor, ele for menor ou igual que 2, quer dizer que esta em um local que pode fazer esse movimento
		copia(arv->matriz, rinf->matriz);// copia uma matriz em outra
		troca(&rinf->matriz[x][y],&rinf->matriz[x+1][y]);// troca os valores
		rinf->heuristica = calculaHeuristica(rinf->matriz);// calcula a heuristica dessa matriz
		rinf->cod = codigo;// atribui um codigo a ela
		codigo++;// incrementa o codigo
		strcpy(rinf->movimento, "MOVER PARA CIMA");// mostra o movimento feito
	} else {//se ao aumentar o valor de x, ele for maior que 2, quer dizer que esta em um local que nao pode fazer esse movimento, entao recebe NULL
		rinf = NULL;
	}
	
	
	if((y+1) <= 2){// se ao aumentar o valor de y , ele for menor ou igual que 2, quer dizer que esta em um local que pode fazer esse movimento
		copia(arv->matriz, rdir->matriz);// copia uma matriz em outra
		troca(&rdir->matriz[x][y],&rdir->matriz[x][y+1]);// troca os valores
		rdir->heuristica = calculaHeuristica(rdir->matriz);// calcula a heuristica dessa matriz
		rdir->cod = codigo;// atribui um codigo a ela
		codigo++;// incrementa o codigo
		strcpy(rdir->movimento, "MOVER PARA ESQUERDA");// mostra o movimento feito
	} else {//se ao aumentar o valor de y, ele for maior que 2, quer dizer que esta em um local que nao pode fazer esse movimento, entao recebe NULL
		rdir = NULL;
	}
	
	if((y-1) >=0){// se ao diminuir o valor de y , ele for maior ou igual que 0, quer dizer que esta em um local que pode fazer esse movimento
		copia(arv->matriz, resq->matriz);// copia uma matriz em outra
		troca(&resq->matriz[x][y],&resq->matriz[x][y-1]);// troca os valores
		resq->heuristica = calculaHeuristica(resq->matriz);// calcula a heuristica dessa matriz
		resq->cod = codigo;// atribui um codigo a ela
		codigo++;// incrementa o codigo
		strcpy(resq->movimento, "MOVER PARA DIREITA");// mostra o movimento feito
	}	else {//se ao diminuir o valor de y, ele for menor que 0, quer dizer que esta em um local que nao pode fazer esse movimento, entao recebe NULL
		resq = NULL;
	}
	
	//Apos isso, foi ligado suas ramificações no em que estava sendo aberto
	
	if(rdir != NULL){ // caso o no for diferente de NULL
	
		if(compara(rdir->matriz,raiz) == 0){ // Vai verificar se já existe uma matriz igual aberta, caso nao tenha, passa esse no a ele
			arv->rdir = rdir; // liga a ramificação
			L = insereIni(rdir->heuristica, L);	// coloca a heuristica na Lista dos não visitados
		}
		else{// caso tenha recebe NULL
			arv->rdir = NULL;	
		}
	}
	else{ // caso seja NULL
		arv->rdir = NULL;
	}
	
	
	if(resq != NULL){// caso o no for diferente de NULL
	
		if(compara(resq->matriz,raiz) == 0){// Vai verificar se já existe uma matriz igual aberta, caso nao tenha, passa esse no a ele
			arv->resq = resq;// liga a ramificação
			L = insereIni(resq->heuristica, L);	// coloca a heuristica na Lista dos não visitados
		}
		
		else{// caso tenha recebe NULL
			arv->resq = NULL;	
		}
		
	}
	else{// caso seja NULL
		arv->resq = NULL;		
	}
	
	
	if(rinf != NULL){// caso o no for diferente de NULL
	
		if(compara(rinf->matriz,raiz) == 0){// Vai verificar se já existe uma matriz igual aberta, caso nao tenha, passa esse no a ele
			arv->rinf = rinf;// liga a ramificação
			L = insereIni(rinf->heuristica, L);	// coloca a heuristica na Lista dos não visitados	
		}
		
		else{// caso tenha recebe NULL
			arv->rinf = NULL;	
		}
		
	}
	else{// caso seja NULL
		arv->rinf = NULL;
	}
	
	
	if(rsup != NULL){// caso o no for diferente de NULL
	
	if(compara(rsup->matriz,raiz) == 0){// Vai verificar se já existe uma matriz igual aberta, caso nao tenha, passa esse no a ele
		arv->rsup = rsup;// liga a ramificação	
		L = insereIni(rsup->heuristica, L);	// coloca a heuristica na Lista dos não visitados	
	}
	
	else{// caso tenha recebe NULL
			arv->rsup = NULL;	
		}
	
	}
	
	else{// caso seja NULL
		arv->rsup = NULL;
	}
	
	
}

void imprimeLista(Lista* L){// Função que imprime a lista...(foi utilizado para verificar se estava inserindo corretamente nas listas)
  Lista* aux = L;
  while(aux!=NULL){
    printf("%d\n", aux->info);
    aux=aux->prox;
  }
}

int verificaAberto(int cod,Lista* L2){ // Verifica se o codigo daquele nó esta na lista dos abertos
	
	while(L2 != NULL){ // percorre a lista
		if(L2->info == cod){ // verifica se o codigo é igual
			return 1; // retorna 1 se for igual
		}
		L2 = L2->prox; // vai para o proximo
		
	}
	
	return 0; // retorna 0 caso não tenha

}


void procura(Arvore* arv,int h,Lista* L2){// procura o no com a heuristica passada
	
	if(arv != NULL){ // verifica se o no não é nulo
	
		int aberto = verificaAberto(arv->cod,L2); // verifica se a matriz desse no ja esta aberto
	
	
		if(arv->heuristica != h || aberto != 0){ // vai procurar recursivamente enquanto a heuristica for diferente ou se ele ja estiver aberto
			procura(arv->rdir,h,L2);
			procura(arv->resq,h,L2);
			procura(arv->rsup,h,L2);
			procura(arv->rinf,h,L2);
		}
		
		else{// achando a heuristica que nao esta aberta
			aux = arv; // auxiliar aponta para esse no
		}
	
	}
}

void verifica(Arvore *arv,Arvore* raiz){ // Função que vai encontrar recursivamente a solução para o problema
	
	int menor = achaMenor(L); // acha a menor heuristica  da lista dos que não foram abertos
	procura(raiz,menor,L2); // vai procurar o no em que a heuristica menor estiver e esse no sera apontado pelo aux
	arv = aux; //arv recebe esse no
	
	if(menor == 0 && arv->heuristica == 0){ // condição de parada, se o menor da lista for a heuristica zero e se da arvore for zero
		printf("\nESTADO FINAL\n\n");
		imprimeMatriz(arv->matriz); // imprime o estado final da matriz
		printf("\nEm %d movimentos!\n",arv->qtdmov); // mostra quantos movimentos foram necessários realizar para chegar no estado final.
	}
	
	else {
	/*	printf("------------------------------------------\n");
		printf("\n %s \n\n", arv->movimento);
		imprimeMatriz(arv->matriz);
		printf("------------------------------------------\n");*/
		arranjaArvore(arv,raiz);// vai abrir as ramificações desse no
		if(menor != 0){
			L = removeLista(L, menor);//vai exlcuir a heuristica desse no na lista de nao abertos(não remove caso seja 0)
		}
		L2 = insereIni(arv->cod,L2);//insere o seu codigo para a lista de abertos.
		verifica(raiz,raiz);	//chama novamente a função até que ela chegue em uma heuristica 0;
	}
	
}


int soluvel(int matriz[][3]){// verifica se é possivel resolver a matriz do seu estado inicial
	
	int inversao=0;
	int i,j;
	int vetor[8];
	int k=0;
	
	//Coloca em um vetor a ordem dos valores em que estão na matriz aleatoria
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			
			if(matriz[i][j] != 9){
				vetor[k] = matriz[i][j];
				k++;
			}
		}
	}
	
	
	//conta quantas vezes um numero antes do outro é maior
	for(i=0;i<8;i++){
		for(j=i+1;j<8;j++){
			if(vetor[j] > vetor[i]){
				inversao++;
			}
		}
	}
	
	// se o numero de inversoes for impar, não é soluvel
	if(inversao%2 == 1){
		return 0; // não é soluvel
	}
	
	//se o numero de inversoes for par, é soluvel
	else{
		return 1; // é soluvel.
	}
	
	
}


int main(){
	
	int h;
	int i,j;
	
	criaMatriz(); // cria uma matriz aleatoria
	
	if(soluvel(matriz) == 0){ // verifica se é soluvel
		printf("\n\tNao e solucionavel!");
	}
	
	else{// caso for
		
		//imprimeMatriz(matriz);
	     //criaMatrizComparacao();
	     printf("\n");
	     //imprimeMatriz(matrizComp);
	     
	     Arvore* raiz = (Arvore*) malloc(sizeof(Arvore)); // aloca um tipo arvore e esse será a raiz da arvore com a matriz aleatoria, o estado inicial
	     for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				
				raiz->matriz[i][j] = matriz[i][j];
				
			}
		}
		
		//atribui NULL para toda sua ramificação
		raiz->rsup = NULL;
		raiz->rinf = NULL;
		raiz->rdir = NULL;
		raiz->resq = NULL;	
		
		raiz->qtdmov=0;// profundidade, o inicio é zero
		
		raiz->cod = codigo; // recebe o codigo
		codigo++;// incrementa o codigo
		raiz->heuristica = calculaHeuristica(raiz->matriz); // calcula a heuristica dessa matriz
		L = insereIni(raiz->heuristica,L); // insere a heuristica na lista 
		strcpy(raiz->movimento, "ESTADO INICIAL");// movimento não é passado
			
		verifica(raiz,raiz); // chama a função que vai achar a solução para essa situação
	}
}
