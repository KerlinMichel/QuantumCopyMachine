CC=gcc
AR=ar

INCLUDE=./include
SRC=$(wildcard src/*.c)
TST=$(wildcard tst/*.c)

all: dynamic static

dynamic:
	$(CC) -fPIC -shared -c $(SRC) -I $(INCLUDE) -o build/qcm.o

static:
	$(CC) -c $(SRC) -I $(INCLUDE) -o build/qcm.o
	$(AR) rcs build/libqcm.a build/qcm.o

test: static
	$(CC) $(TST) -L build -lqcm -lpng -o ./build/test -I $(INCLUDE)
	./build/test

clean:
	rm -rf ./build/qcm.o ./build/libqcm.a ./build/test