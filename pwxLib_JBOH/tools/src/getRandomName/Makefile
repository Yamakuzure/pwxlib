.PHONY: all clean

CXX       = g++
CXXFLAGS += -I../../..
LDFLAGS  +=
TARGET    = ../../getrn
MODULES   = environment.o tools.o main.o

all: $(TARGET)

clean:
	@echo "Cleaning getRandomName"
	@rm -rf $(TARGET) $(MODULES)


$(TARGET): $(MODULES)
	@echo "Linking $(TARGET) ..."
	@$(CXX) -o $(TARGET) $(LDFLAGS) $(MODULES)

%.o: %.cpp
	@echo "Generating $@ ..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# --- Dependencies ---
%.cpp: %.h
main.h: environment.h tools.h
tools.h: main.h Args.h
