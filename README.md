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
  
  **IMPORTANTE:** O binário deve estar no diretório bin
  
  * Envie uma mensagem de desligamento do escalonador com o comando `./shutdown`
  
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
Em linhas gerais, o que o escalonador é responsável por fazer?

## Tratamento de mensagens
### ExecuteProgramPostponed
O que o escalonador faz quando recebe um ExecuteProgramPostponed?

### NotifyScheduler
O que o escalonador faz quando recebe um NotifyScheduler?

### Shutdown
Configura uma *flag* que diz se o sistema deve ser finalizado. Toda vez após o tratamento **de qualquer mensagem**, o escalonador verifica se essa flag está marcada. Se sim, verifica se todos nós estão livres. Caso não estejam, não termina o processo e espera a disponibilidade dos nós ocupados. Caso todos os nós estejam livres, o escalonador mata os processos filhos(gerentes), imprime a lista de programas que não foram executados, caso existam, e imprime as estatísticas de execução.

### Alarm
O que o escalonador faz quando recebe um Alarm?

# Nó (Gerente)
Em linhas gerais, o que o nó é responsável por fazer? Falar da construção da vizinhança

## Tratamento de mensagens
### ExecuteProgramPostponed
O que o nó faz quando recebe um ExecuteProgramPostponed?
### NotifyScheduler
O que o nó faz quando recebe um NotifyScheduler?
### Timestamp
O que o nó faz quando recebe um Timestamp?
