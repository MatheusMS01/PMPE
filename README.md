# PMPE
Postponed Multi-Process Execution

# Compilando o projeto
- Para compilar todo o projeto, direcione o seu terminal para o diretório PMPE, e execute o comando `make`
- Para compilar um projeto em específico, direcione o seu terminal para o diretório em específico, e execute o comando `make`

# Simulando o projeto
**Compile antes o projeto!**
- Abra três janelas em um terminal a partir da raiz do projeto PMPE
  - Primeiro terminal direcionado em `/scheduler`
    - Execute o comando `./escalonador`. Será executado o programa escalonador responsável por gerenciar todo o processo de execução dos gerentes
  - Segundo terminal direcionado em `/execute_postponed`
    - Execute o comando `./executa_postergado` delay program_name, onde delay é um inteiro positivo, relativo ao tempo que o escalonador deve esperar antes de pedir aos gerentes que executem o programa(program_name), para inicializar a execução do programa pelos gerentes.
  - Terceiro terminal direcionado em `/shutdown`
    - Execute o comando ./shutdown para finalizar a execução do projeto
