/ara a meta 1 temos variáveis ambiente e queremos que o balcão corra o classificador e comunique com ele

-» 'comando ps' mostra todos os processos em execução na máquina
-» 'comando ps a' mostra todos os processos do meu terminal
-» 'comando ps ax' mostra todos os processos do sistema
-» 'comando ps l' mostra mais informações sobre os processos que estão a correr(pid, paipid)

-» 'comando kill' mata completamente a aplicação mesmo se o o ctrl c nao funcionar
	-> kill -9 pid(id do processo, fazendo ps l conseguimos ver)

-» 'ctrl z' suspende o processo, não o termina porque ele continua a ocupar cpu, no entanto não corre
	-> para retomar fazer fg %x(numero da tarefa que queremos, se não metermos nada vai ser a ultima, com jobs vemos quais existem)
		-> também podemos fazer com o bg só que aí não conseguimos fazer comandos visto que com bg não temos acesso a teclado, apenas com fg 

	-> para matar as suspensas, fazer kill %x

'Tarefas'
-» 'correr aplicação por for ground, bg' é quando somos nos a correr com o teclado, monitor
-» 'correr em back groung, bg' quando mandamos aplicação correr por trás
-» 'comando jobs' mostra as aplicações que estão suspensas naquele terminal


-» 'o exec faz mudar de processo', ou seja, se começarmos com o processo A e fizermos exec para o B e esse exec correr bem vamos mudar para o B e morrer com o B
	-> não conseguimos continuar com o programa A, se tivermos prints desse programa A, já não aparecemos, mudámos completamente para o B
	-> o número do processo continua o mesmo, mas o conteúdo muda.

-» 'se quisermos continuar com o programa A' e ter o B a correr na mesma sem matar o A, temos de ter um filho a correr o B  


					
																				DIA 09/11
-»'ficha3 ex5'
	
int a = 10;
if (fork() == 0)	//-> filho
	//fazemos aqui o exec porque aqui estamod dentro do filho e estragamos o filho
 	a++;
else	//-> pai 
 	a--;

 //aqui tanto mostra o pai como o filho
printf(“\na = %d\n”, a);
return 0;


																				DIA 16/11
-» 'Tabela de descritores'
	-> 0 - stdin(teclado)  
	-> 1 - stdout(monitor)
	-> 2 - stderr
	-> 3 - resultado do open

-» open
	-> fd = open("dados.txt", O_WRONLY|O_CREAT, 00644); 
		-> O_CREAT para criar o ficheiro se não existir com as permissões 6(RW-)4(R--)4(R--) 


-> 'para o pai enviar'
	-> close(0) - stdin não quer ler
	-> dup(canal[0])
	-> close(canal[0])
	-> close(canal[1])

	-> faz write no canal[1] 

		-> canal[1] -write- (=====) -read- canal[0]

-> 'para o pai ler'
	-> close(1) - stdout não quer escrever 
	-> dup(retorno[1])
	-> close(retorno[0])
	-> close(retorno[1])

	-> faz read no retorno[0]

		-> retorno[0] -write- (=====) -read- retorno[1]

