CC = g++ -std=c++17

# INCLUDES = -I/usr/local/Cellar/sfml/2.5.1_1/include/

CFLAGS = -g

LDFLAGS = -L/usr/local/Cellar/sfml/2.5.1_1/lib/

MAINOBJS = main.o zombie.o

all: main

main: $(MAINOBJS)
	$(CC) $(MAINOBJS) $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system -o main


zombie.o: zombie.cpp zombie.h
	$(CC) $(CFLAGS) -c zombie.cpp

clean:
	rm -f $(SERVEROBJS) $(CLIENTOBJS) *~