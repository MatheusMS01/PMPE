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
O escalonador cria os 16 processos gerentes de execução usando a chamada de sistema `fork()`, configura os nós inicialmente como livres e espera receber uma mensagem pelo mecanismo IPC de fila de mensagem.  A partir do momento em que há mensagens na fila, existem quatro formas de tratamento das mensagens que consistem em executar o programa de forma postergada, receber a notificação do término da execução, receber um alarme informando que o delay de execução em relação a hora atual já passou e finalizar a execução de todos os processos.

## Tratamento de mensagens
### ExecuteProgramPostponed
Verifica se o tempo para executar o programa já foi alcançado. Se não, um `alarm()` será inicializado com o ExecuteProgramPostponed que deve ser executado em menos tempo e então será inseridos na lista de execuçôes pendentes, uma mensagem destinada a cada gerente caso ela já não exista. Caso o tempo tenha sido alcançado, uma mensagem com os argumentos do protocolo ExecutePostponedProtocol e cada **gerente livre** como destino é escrita na fila de mensagens para o nó zero, modifica-se o estado do gerente para ocupado no mapa de nós e o protocolo é retirado da lista de pendências caso lá ele esteja. Se o gerente não estiver livre então será adicionada a mensagem à lista de pendências, caso lá ela já não esteja.

### NotifyScheduler
O que o escalonador faz quando recebe um NotifyScheduler?

### Shutdown
Configura uma *flag* que diz se o sistema deve ser finalizado. Toda vez após o tratamento **de qualquer mensagem**, o escalonador verifica se essa flag está marcada. Se sim, verifica se todos nós estão livres. Caso não estejam, não termina o processo e espera a disponibilidade dos nós ocupados. Caso todos os nós estejam livres, o escalonador mata os processos filhos(gerentes), imprime a lista de programas que não foram executados, caso existam, e imprime as estatísticas de execução.

### Alarm


# Nó (Gerente)
Em linhas gerais, o que o nó é responsável por fazer? Falar da construção da vizinhança

## Tratamento de mensagens
### ExecuteProgramPostponed
O que o nó faz quando recebe um ExecuteProgramPostponed?
### NotifyScheduler
O que o nó faz quando recebe um NotifyScheduler?
### Timestamp
O que o nó faz quando recebe um Timestamp?
