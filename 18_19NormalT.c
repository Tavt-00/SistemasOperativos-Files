1. O /etc/shadow contém as passwords dos utilizadores. Este ficheiro pode ser lido e escrito apenas pelo administrador e nunca pelos utilizadores (protegendo assim de ataques forçados para obter passwords).
Mas há sempre forma de atacar o sistema e contrariar essa “protecção”: setuid ou o sudo.
Se o setuid ligado , do dono do executável -  o processo pode fazer o que o utilizador dono também pode,  se estiver desligado (de quem lança o processo em causa), esse só pode fazer o que o utilizador também pode.
O comando sudo permite executar os comandos que sejam específicos a utilizadores também específicos, corre com privilégios de administrador. Este comando executa comandos com privilégios elevados.


2.Os processos são compostos por: Código, Dados, Heap e Pilha. 
Como sabemos o espaço destas duas últimas não pode ser determinado inicialmente, expandindo-se sempre que necessário.
Existe um espaço entre ambas as zonas que não tem nada, e nem sequer está mapeado em memória. Pode acontecer que ao fazer int *p (ponteiro não inicializado) o p fique a apontar para uma zona de memória qualquer. 
O mais provável é ficar a apontar para aquela zona sem nada.

Mais tarde o processador vai tentar inicializar o ponteiro p com 200.
Vai tentar perceber onde é que a memória (responsável pelo p) está em memória real, chegando à conclusão que não se encontra em lugar algum acabando por lançar uma interrupção. 
CASO o p fique a apontar para uma zona de código, também haveria problemas: como sabemos a zona de código só dá para leitura. Logo apenas funcionaria bem se o p ficasse a apontar para a pilha ou heap (podem ter leitura e ESCRITA).