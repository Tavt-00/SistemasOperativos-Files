5.

int main(int argc, char const *argv[]){
	
	char mensagem[10];

	do{
		printf("Escreva a cadeia de caracteres");
		scanf("%s", mensagem);

		//chamar funcao

	}while(strcmp(mensagem, "fim") == 0);

	return 0;
}