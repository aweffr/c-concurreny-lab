DIR_SRC = ./src
DIR_INC = ./include
DIR_PKG = ./pkg
DIR_BIN = ./bin

CFLAG = -pthread -g -std=c99 -Wall -I${DIR_INC}

helloworld: helloworld.o
	gcc ${DIR_PKG}/helloworld.o -o ${DIR_BIN}/helloworld ${CFLAG}

helloworld.o:
	gcc -c -o ${DIR_PKG}/helloworld.o ${DIR_SRC}/helloworld.c ${CFLAG}