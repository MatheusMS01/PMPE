# PMPE
Postponed Multi-Process Execution

# Compilando o projeto
* Direcione o seu terminal para o diretório PMPE
  * Para compilar o projeto, execute o comando `make`
  * Para limpar o projeto, execute o comando `make clean`
  
# Executando o projeto
* Direcione o seu terminal para o diretório PMPE/bin
  * Inicie o escalonador com o comando `./escalonador &`. Ele será executado em **background**
  * Envie uma mensagem de execução de um programa com o comando `./executa_postergado delay program_name`, sendo `delay` um inteiro sem sinal e `program_name` uma string identificando o nome do binário que deverá ser executado. 
  
  **IMPORTANTE:** O binário deve estar no diretório bin. O programa de teste é o `hello_node` e seu código encontra-se no diretório PMPE/hello_node
  
  * Envie uma mensagem de desligamento do escalonador com o comando `./shutdown`
  
### **ATENÇÃO**
  
Se o executa_postergado estiver imprimindo a mensagem de erro "*Failed to write*", a fila de mensagem está com algum erro. Para resolver execute os seguintes comandos:
 
`pkill escalonador`

`ipcrm -Q 0x727dda9`

E inicie novamente o escalonador
  
# Protocolos
Os protocolos deste projeto são tipos de mensagens necessária para diferenciação quando escritas/lidas na fila de mensagem. Todos protocolos tem um Id, e possuem a seguinte característica:
* São delimitados por um caracter ';'
* O primeiro valor refere-se ao Id do protocolo
* Os demais valores referem-se aos parâmetros do protocolo

### Exemplo:

;1;2;3;Abcd;

1 = Id do protocolo

2 = Primeiro parâmetro

3 = Segundo parâmetro

Abcd = Terceiro parâmetro

## ExecuteProgramPostponedProtocol (Id = 0)
Protocolo responsável por notificar o escalonador a execução de um programa

### Parâmetros
* SequentialNumber: Inteiro de identificação único da mensagem
* Delay: Inteiro que define o tempo mínimo que o escalonador deve esperar antes de enviar as execuções para os nós
* ProgramName: String que define o nome do programa a ser executado
* DestinationNode: Inteiro que define qual nó deve executar o programa
* SubmissionTime: Inteiro longo que define qual horário a execução foi enviada para o escalonador

## NotifySchedulerProtocol (Id = 1)
Protocolo responsável por notificar o escalonador sobre o término da execução do programa por algum nó

### Parâmetros
* NodeId: Inteiro que define qual nó enviou a notificação
* SubmissionTime: Inteiro longo que define qual horário a execução foi enviada para o escalonador
* BeginTime: Inteiro longo que define qual horário a execução foi iniciada pelo nó
* EndTime: Inteiro longo que define qual horário a execução foi terminada pelo nó
* Delay: Inteiro que define o tempo mínimo que o escalonador deve esperar antes de enviar as execuções para os nós
* ProgramName: String que define o nome do programa a ser executado
* PID: Inteiro que define o ID do processo
* Success: Booleano que define se a execução ocorreu com sucesso pelo nó

## Shutdown (Id = 2)
Protocolo responsável por notificar o escalonador o término da execução do mesmo

### Parâmetros
Não possui parâmetros

## TimestampProtocol (Id = 3)
Protocolo responsável por notificar o nó que seu filho terminou a execução do programa. É enviado quando o processo captura um sinal SIGCHLD.

### Parâmetros
* Timestamp: Inteiro longo que define qual horário a execução foi terminada pelo filho
* Success: Booleano que define se a execução ocorreu com sucesso pelo filho

## AlarmProtocol (Id = 4)
Protocolo responsável por notificar o escalonador que o temporizador da chamada de sistema `alarm()` estourou. É enviado quando o processo captura um sinal SIGALRM.

### Parâmetros
Não possui parâmetros

# Escalonador
O escalonador cria os 16 processos gerentes de execução usando a chamada de sistema `fork()`, configura os nós inicialmente como livres e espera receber uma mensagem pelo mecanismo IPC de fila de mensagem para seu tipo.  A partir do momento em que há mensagens na fila, existem quatro formas de tratamento das mensagens que consistem em executar o programa de forma postergada, receber a notificação do término da execução, receber um alarme informando que o delay de execução em relação a hora atual já passou e finalizar a execução de todos os processos.

## Tratamento de mensagens
### ExecuteProgramPostponed
Verifica se o tempo para executar o programa já foi alcançado. Se não, um `alarm()` será inicializado com o ExecuteProgramPostponed que deve ser executado em menos tempo e então será inseridos na lista de execuçôes pendentes, uma mensagem destinada a cada gerente caso ela já não exista. Caso o tempo tenha sido alcançado, uma mensagem com os argumentos do protocolo ExecutePostponedProtocol e cada **gerente livre** como destino é escrita na fila de mensagens para o nó zero, modifica-se o estado do gerente para ocupado no mapa de nós e o protocolo é retirado da lista de pendências caso lá ele esteja. Se o gerente não estiver livre então será adicionada a mensagem à lista de pendências, caso lá ela já não esteja.

### NotifyScheduler
Ao receber essa mensagem, o escalonador configura o nó que enviou a mensagem como livre.

Caso o programa tenha sido executado com sucesso pelo processo(parâmetro da mensagem), calcula-se o *makespan*, que é o tempo final de execução menos o tempo inicial, adiciona-se na mensagem os parâmetros *job*, nome do arquivo, delay e makespan, e são impressos na tela. Porém, caso haja falha na execução, é impressa uma mensagem de erro.

E, por fim, é verificado se o nó que enviou essa mensagem possui alguma pendência de execução. Caso exista, trata novamente a mensagem de ExecuteProgramPostponed que estava na lista de execuções pendentes.

### Shutdown
Configura uma *flag* que diz se o sistema deve ser finalizado. Toda vez após o tratamento **de qualquer mensagem**, o escalonador verifica se essa flag está marcada. Se sim, verifica se todos nós estão livres. Caso não estejam, não termina o processo e espera a disponibilidade dos nós ocupados. Caso todos os nós estejam livres, o escalonador mata os processos filhos(gerentes), imprime a lista de programas que não foram executados, caso existam, e imprime as estatísticas de execução.

### Alarm
Ao receber essa mensagem, são tratadas todas as mensagens que se encontram na lista de execuções pendentes, respeitando o limite máximo de *bytes* que podem ser escritos na fila de mensagem.

# Nó (Gerente)
Espera receber uma mensagem pelo mecanismo IPC de fila de mensagem, para seu tipo. Ao receber uma mensagem, trata-a como visto abaixo.

## Tratamento de mensagens
### ExecuteProgramPostponed
O que o nó faz quando recebe um ExecuteProgramPostponed?
### NotifyScheduler
O que o nó faz quando recebe um NotifyScheduler?
### Timestamp
Ao receber essa mensagem, que indica que o seu filho encerrou a execução, o nó configura o tempo de término da execução(EndTime) e a *flag* de sucesso(Success), indicando se a execução teve sucesso ou não. Em seguida, é feito o roteamento(seguindo a topologia *torus*) de um NotifyScheduler para o nó zero, responsável por transmitir a mensagem ao escalonador.

# Executa Postergado
Este processo é responsável transmitir uma mensagem ao escalonador que um novo programa deve ser executado pelos nós gerentes. Como explicado no começo do documento, o padrão `./executa_postergado delay program_name` é utilizado para enviar ao escalonador a execução de todos os nó do programa `program_name` para ser executado após `delay` segundos. Os parâmetros recebidos por argumentos são convertidos para a string do protocolo ExecutePostponedProtocol e adicionadas à fila de mensagens para o escalonador, além do tempo em que a mensagem foi gerada.
