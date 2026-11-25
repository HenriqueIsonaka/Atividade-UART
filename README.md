# Atividade Controle de LEDs pela UART

Dupla: 
Henrique Isonaka - NUSP:17972455
Pedro Domingues - NUSP:17918742 

OBS: O VSCODE do Pedro não está funcionando apropriadamente, então, para esta atividade, todos os testes e commits foram feitos a partir do VSCODE do Henrique, mesmo todos os testes tendo sido feitos em dupla.

Link github: https://github.com/HenriqueIsonaka/Atividade-UART

O código foi feito a partir de um modo orientado à interrupção da UART0. Como a função de leitura da UART0 só conseguia ler 1 byte a cada vez que era chamada, foi necessário fazer com que a cada byte que fosse recebido,  a função de interrupção fosse executada uma vez, sendo que quando fossem completados 6 bytes, seria  executado um trecho de compreensão desses dados enviados, que seriam tratados como um pacote, sendo eles armazenados em um buffer.

O ID escolhido pela placa foi 3 (ou 0b10)

Os LEDs e seus estados no pacote foram armazenados em dois buffers, LEDs[] e cores[], para ser possível saber se um LED executaria ou não a ação selecionada no pacote na hora que em que ela fosse executada.

Na parte de compreensão do pacote  são determinados os estados de cada LED e o valor esperado pelo CHECK. Na primeira etapa, se o valor do byte do LED for zero, ele não responderá à ação selecionada no pacote, mas se ele for  qualquer outro valor, ele a realizará. Na segunda etapa, é calculado valor esperado de checagem na hora de terminar de escrever o pacote, que, em binário, são 3 zeros seguidos do xor do quinto byte, seguido do xor do quarto byte, seguido do xor do terceiro byte, seguido do xor do segundo byte, seguido do xor do primeiro byte do pacote. Para determinar é necessário verificar se o valor do byte é 0 ou 256. Se ele for, o valor do xor é 0, e se ele não for, o valor do xor é 1.



Se o ID não for o da placa, a placa não terá alterações no LED e será imprimida a mensagem "ERRO: ID não identificado!". Se o CHECK estiver incorreto, a placa também não terá alterações no LED e será imprimida a mensagem "ERRO: CHECK incorreto!".
Caso nenhum desses casos aconteça, os LEDs com bytes de valores diferentes de zero realizarão a ação selecionada, sendo isso feito a partir de um for que só executa a ação dos LEDs que tiverem o valor de 1 em seu correspondente no vetor cores[].

<img width="426" height="240" alt="erro1" src="https://github.com/user-attachments/assets/c585c335-1b26-41ee-b011-d462dd598a6b" />
