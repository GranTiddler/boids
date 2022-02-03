output: main.o
	g++ main.o -o boids -lsfml-graphics -lsfml-window -lsfml-system
	./boids


main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o


target: dependencies
	action