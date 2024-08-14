all: dht22

dht22: dht22.c
    ${CC} ${CFLAGS} -o dht22 dht22.c
