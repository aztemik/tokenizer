CXX = clang++
CXXFLAGS = -std=c++17 -Wall



# Lista de todos los archivos .cpp que componen tu proyecto
SOURCES = main.cpp \
		manipulateBinaryFiles/BinaryFileAHandler.cpp \
		manipulateBinaryFiles/BinaryFileBHandler.cpp \
		manipulateBinaryFiles/BinaryFileCHandler.cpp \
		manipulateBinaryFiles/GeneralBinaryFileHandler.cpp \
		tokenizer/tokenizer.cpp \
		manipulateBinaryFiles/specialTokens.cpp

# Genera los nombres de los archivos objeto (.o)
OBJECTS = $(SOURCES:.cpp=.o)

# Nombre del ejecutable final
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
