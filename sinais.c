//VER ONDE FIZ OS EXERCICIOS COM ISTO

-» man 7 signal -> para ver o MANUAL dos sinais seguintes:

sigint() -> por default é equivalente ao "ctrl + c", termina a aplicacao

sigstop() -> por default equivalente ao "ctrl + z", pausa a app 

sigcont()  -> equivalente ao "fg" continua a app depois desta ter sido pausada

sigkill()  -> matar o processo

sigusr1() e sigusr2() -> usado para qualquer coisa

sigalarm()  -> associado ao alarm(), mais para temporização

sigchild()  -> sinal que manda fazer algo quando o filho de um processo termina


-» man 2 signal ou pause ou kill ou alarm para os sianis seguintes:

signal(a,b) -> quando recebe o sinal "a", faz o que estiver em "b"
	-> se nao tivermos isto, a dizer o que fazer com o sinal "a", então quando mandarmos um sinal, a app vai fazer o que estiver por default desse sinal  //->ex2.c ficha4

alarm(x) -> conta x segundos e no fim manda um SIGALRM    //->ex2.c ficha4

pause() -> nao faz nada a partir dali

kill(pid, s) -> envia para o processo com o pid que está em "pid" o sinal que está em "s", aqui nao conseguimos enviar mais nada, nem um valor (sigqueue pode)  //->enviaKill.c ficha4


-»enviar e receber sinais com mais informações

sigaction(sinal, estrutura, old) -> parecido ao signal() mas podemos usar uma estrutura já definida(act) que nos diz mais informações (quem enviou o sinal, com que valor, etc)	//->ex4.c ficha4
	-> 1º paramettro é o nome do sinal que é para tratar
	-> 2º parametro é a estrutura do sigaction
	-> 3º é o comportamento antigo

sigqueue(pid, sinal, value) ->parecido ao kill() mas podemos enviar um valor para alem de o signal		//->enviaSigque.c ficha4
	-> 1º parametro é o pid do processo para onde enviar
	-> 2º paramettro é o nome do sinal que é para enviar
	-> 3º é o valor a enviar para alem do sinal