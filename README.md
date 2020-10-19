# Projeto de Implementação 1 - Sistemas Operacionais

Participantes do grupo: Victor Luís Aguilar Antunes, Eduardo dos Santos Gualberto, Lucas Vinícius Domingues e Rafael Yoshio Yamawaki Murata.

## Instalação

```bash
git clone https://github.com/eduardo-gualberto/ProjetoImplementacao1_SO.git
cd ProjetoImplementacao1_SO/
git clone https://github.com/SMFSW/cQueue.git
```
ou baixar os arquivos do repositrio zipados

Obs.: Usa-se para uma das implementaçes uma [biblioteca terceira](https://github.com/SMFSW/cQueue) que implementa e cria uma interface de gerenciamento de Filas.

## Execução

Para execução,  provável que antes deva-se dar permissão de execução do arquivo "compileandrun.sh"

```bash
chmod +x compileandrun.sh
```
Feito isso, basta executar o arquivo e passar como único e indispensável argumento se deseja executar a implementação 1 ou 2. Exemplo abaixo:

```bash
./compileandrun.sh 1
.
.
.
./compileandrun.sh 2
.
.
.
./compileandrun.sh 3
.
.
.
./compileandrun.sh 4
```
Os comandos acima resultarão no comportamento padrão de cada implementação. Para especificar o números de assentos e de clientes no bar, passe 2 argumentos extras para ./compileandrun. Exemplo:

```bash
./compileandrun.sh 1 3 10
```

O primeiro argumento, como dito anteriormente, refere-se a implementação que deseja-se executar. O segundo refere-se a quantidade de assentos que terá no sushi bar. O terceiro e último argumento refere-se a quantidade de clientes que estarão no bar.
