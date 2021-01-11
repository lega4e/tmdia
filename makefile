EXECUTABLE = build/main
CFLAGS = -std=gnu++17 -c -Wall -g3 -DDEBUG
LDFLAGS =
LIBS = -lsfml-graphics -lsfml-system -lsfml-window





# main
all: $(EXECUTABLE)

run: $(EXECUTABLE)
	$(EXECUTABLE) < input.json


re: clean $(EXECUTABLE)

rerun: clean $(EXECUTABLE)
	$(EXECUTABLE)



$(EXECUTABLE): build/main.o
	g++ $(LDFLAGS) -o $(EXECUTABLE) build/main.o $(LIBS)

build/main.o: main.cpp
	g++ $(CFLAGS) -o build/main.o main.cpp





# clean
clean:
	-rm *.o $(EXECUTABLE)





# end
