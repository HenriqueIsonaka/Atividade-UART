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
Caso nenhum desses casos aconteça, os LEDs com bytes de valores diferentes de zero realizarão a ação selecionada, sendo isso feito a partir de um for que só executa a ação dos LEDs que tiverem o valor de 1 em seu correspondente no vetor cores[]. É importante destacar que se o valor do byte da ação não corresponder a nenhum dos valores de 1 a 5, a placa também não terá alterações no LED, porém não aparecerá nenhuma mensagem de erro.

Alguns exemplos de funcionamento:

<img width="426" height="240" alt="id" src="https://github.com/user-attachments/assets/ccca4c11-3353-4853-b337-214820dceea6" />

ERRO de ID não identificado.

<img width="426" height="240" alt="CHECK" src="https://github.com/user-attachments/assets/5e80fda9-803d-4d02-9703-3e75c644e8e3" />

ERRO de CHECK incorreto.

<img width="426" height="240" alt="Ligar" src="https://github.com/user-attachments/assets/1b08ae44-8292-45f4-9249-d6b71b3b9dbf" />

Ação de ligar com LEDs azul e verde selecionados.

<img width="426" height="240" alt="toggle" src="https://github.com/user-attachments/assets/491b4595-0586-4215-aa9b-8bb96c42ccc3" />

Ação de toggle com LEDs vermelho e azul selecionados.

<img width="426" height="240" alt="1s" src="https://github.com/user-attachments/assets/605322b9-a986-4471-af97-10eefff1501e" />

Ação de piscar por 1 segundo com todos os LEDs selecionados (por problemas da câmera, LED branco ficou muito rosa).
