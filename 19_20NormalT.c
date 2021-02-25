4.
deslocamento de 9 bits

a. Endereço real = 2058 / tamanho da página
	-> tamanho da pagina = 2^9=512
	-> 2058 / 512 = 4 com resto de 10, ir ao indice 4 e somar 10 - 1072 + 10 = 3082 

3082 de Endereço real

b.  __asm___ { " HLT " }; está a chamar um módulo do kernel , que irá por o núcleo a "dormir" até à próxima interrupção.
Assim, a função faz, apenas faz algo até à diretiva __asm___


5. O uso de variáveis globais é considerado uma má prática e como temos duas threads que necessitam de aceder às ditas variáveis globais, o uso de uma estrutura onde estariam as mesmas variáveis 
e o uso de mutexs para controlar o acesso a essas mesmas variáveis da estrutura por parte de cada thread, seria o mais apropriado.