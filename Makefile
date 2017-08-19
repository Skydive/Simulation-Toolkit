CC=gcc
CFLAGS=$(pkg-config tcc.pc --cflags --libs)
OBJ=Main.o

%.o: %.c
	$(CC) -o build/$@ -c src/$< $(CFLAGS)

all: $(OBJ)
	gcc -o build/$@ src/$^ $(CFLAGS)
