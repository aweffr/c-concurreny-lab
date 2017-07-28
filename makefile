DIR_SRC = ./src
DIR_INC = ./include
DIR_PKG = ./pkg
DIR_BIN = ./bin

CFLAG = -pthread -g -std=c99 -Wall -I${DIR_INC} -D_GNU_SOURCE

# helloworld: helloworld.o
# 	gcc ${DIR_PKG}/helloworld.o -o ${DIR_BIN}/helloworld ${CFLAG}

# helloworld.o:
# 	gcc -c -o ${DIR_PKG}/helloworld.o ${DIR_SRC}/helloworld.c ${CFLAG}

counter: counter.o
	gcc ${DIR_PKG}/counter.o -o ${DIR_BIN}/counter ${CFLAG}

counter.o:
	gcc -c -o ${DIR_PKG}/counter.o ${DIR_SRC}/counter.c ${CFLAG}

link_list: link_list.o
	gcc ${DIR_PKG}/link_list.o -o ${DIR_BIN}/link_list ${CFLAG} 

link_list.o:
	gcc -c -o ${DIR_PKG}/link_list.o ${DIR_SRC}/link_list.c ${CFLAG}

clean:
	find ${DIR_PKG} -name '*.o' -exec rm -f {} \;
	find ${DIR_BIN} -name '*' -exec rm -f {} \;

all:
	make counter link_list