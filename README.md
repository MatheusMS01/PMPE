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
  
# Protocolos:

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
Protocolo responsável por notificar o nó que seu filho terminou a execução do programa.

### Parâmetros
* Timestamp: Inteiro longo que define qual horário a execução foi terminada pelo filho
* Success: Booleano que define se a execução ocorreu com sucesso pelo filho

## AlarmProtocol (Id = 4)
Protocolo responsável por notificar o escalonador que o temporizador da chamada de sistema alarm() estourou

### Parâmetros
Não possui parâmetros
