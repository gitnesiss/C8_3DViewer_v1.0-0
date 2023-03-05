prog: clean
	g++ -c test.cpp
	g++ test.o -o my_test -lGLU -lGLEW -lglut -lglfw -ldl -lGL 

clean:
	rm -Rf test.o
	rm -Rf my_test