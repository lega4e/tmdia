EXECUTABLE = main
BUILD_PATH = ../build
CFLAGS     = -std=gnu++17 -c -Wall -g3 -DDEBUG -I./lib
LDFLAGS    =
LIBS       = -lsfml-graphics -lsfml-system -lsfml-window





# main
all: $(EXECUTABLE)

run: $(BUILD_PATH)/$(EXECUTABLE)
	$(BUILD_PATH)/run


re: clean all

rerun: clean $(BUILD_PATH)/$(EXECUTABLE)
	all



$(EXECUTABLE): $(BUILD_PATH)/main.o $(BUILD_PATH)/define.o
	g++ $(LDFLAGS) -o $(BUILD_PATH)/$(EXECUTABLE) \
		$(BUILD_PATH)/main.o $(BUILD_PATH)/define.o $(LIBS)

$(BUILD_PATH)/main.o: main.cpp TimeDia.hpp
	g++ $(CFLAGS) -o $(BUILD_PATH)/main.o main.cpp

$(BUILD_PATH)/define.o: define.cpp TimeDia.hpp
	g++ $(CFLAGS) -o $(BUILD_PATH)/define.o define.cpp





# clean
clean:
	-rm $(BUILD_PATH)/*.o $(BUILD_PATH)/$(EXECUTABLE)





# end
