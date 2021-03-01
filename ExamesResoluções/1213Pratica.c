#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

//a)
int main(int argc, char const *argv[])
{
	char pass[30];
	int tempo;

	if(argc==3){
		strcpy(pass, argv[1]);
		tempo = atoi(argv[2]);
	}
	else
		exit(1);

	
	if (access("CRACKER", F_OK)==0){
		printf("Favor esperar, maquina a correr\n");
		exit(1);
	}

	return 0;
}


typedef struct estrutura estru:
struct estrutura{
	char letra;
	int matriz[10];
}

typedef struct passe pass:
struct passe{
	char letra;
	int matriz[10];
}

//b)
int main(int argc, char const *argv[])
{
	char pass[30];
	int tempo;

	if(argc==3){
		strcpy(pass, argv[1]);
		tempo = atoi(argv[2]);
	}
	else
		exit(1);

	mkfifo("CRACKER", 0600);

	int res, estado, res_size, filhos[27];
	estru ola;
	estru.letra = 'a';
	strcpy(estru.matriz,"olaokaola");

	for(int i = 0; i<27, i++){
		res = fork();

		if (res==0){
			filhos[i]=getpid();
			execl("testa","testa", NULL);
			exit(1);
		}
		wait(&estado);						

		int fd_open;
		fd_open = open("TESTA_FIFO", O_RDWR);
		
		res_size = write(fd_open,&ola, sizeof(ola));
	}

	//C
	pass resposta; 

	int cracker = open("CRACKER", 0_RDWR);
	read(cracker, &pass, sizeof(password));

	for (j = 0; j < 27; j++){
		kill(filhos[i], SIGUSR1);
	}

	printf("password: %s", respsta.matriz);

	unlink("CRACKER");


	return 0;

}