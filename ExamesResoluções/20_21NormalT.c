2.
a. Partes menos boas neste código, são o uso de varáveis globais (algo que nunca é uma boa prática) e com isto, o não uso de mutex para que as threads tenham um acesso mais organizado ao conteúdo de varíaveis partilhadas entre elas.

b.

//tirando as variávies globais..

//uso de uma estrutura comum

typedef struct
{
	int novo;
	int mersn;
	pthread_mutex_t *ptrinco;
}TDATA;

sendo assim, como o uso desta estrutura na criação das threads, anteriormente (no codigo main) seria necessário inicializar as variávies "novo" e "mersn" a 0.

//uso do mutex nas duas threads

//threa de pesquisa

for (int i = 0; i < 1000000; i++){
	
	if(eMersenne(i)){
		pthread_mutex_lock(data->ptrinco);

		novo = 1;
		mersn = i;

		pthread_mutex_unlock(data->ptrinco);
	}
}

pthread_mutex_lock(data->ptrinco);

novo = 1;
mersn = -1;

pthread_mutex_unlock(data->ptrinco);


//na thread apresenta

while(1){
	while(novo == 0);
	if (mersn == -1){	
		break;
	}
	printf("%d\n",mersn);
	
	pthread_mutex_lock(data->ptrinco);
		novo = 0;
	pthread_mutex_unlock(data->ptrinco);
}


3.
Máquina tem 10Kb de RAM, cada página tem 2Kb.
Pela figura vemos que temos uma memória de 5 páginas.
2*5 = 10 e como a máquina tem 10Kb de RAM, não há espaço para mais.

Se no istante t2 surge a necessidade de encaixar uma nova página e memória já está cheia, segundo o algorítmo NRU (Not Recently Used), os bits R e M serão analisados para classificar as páginas quanto à sua utilização.
O bit R está a 1 quando a página é acedida e o M a 1 sempre que a página é modificada.

Se no instante t1 ocorre uma leitura nas páginas 0,2 e 4, estas foram utilizadas recentemente e sendo assim, a escolha da página que irá sair para dar lugar à pretendida no instante T2, resume-se entre a página 1 e 3.
Perante isto, apenas a página 1 tem o bit R e M ainda ambos a 0, o que significa que pelo NRU seria esta a ser escolhida.