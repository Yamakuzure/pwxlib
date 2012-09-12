.PHONY: all clean

CXX       = g++
CXXFLAGS += -I../../.. -DPWX_THREADS
LDFLAGS  +=
LIBS      = -lsfml-system -lsfml-window -lsfml-graphics
TARGET    = ../../mkst
MODULES   = callback.o consoleui.o renderer.o sfmlui.o textbox.o environment.o tools.o workloop.o main.o wzseq.o

# Add font config if configured
ifneq (DEFAULT, ${FONT_PATH})
	CXXFLAGS := ${CXXFLAGS} -DFONT_PATH=\"${FONT_PATH}\"
endif
ifneq (DEFAULT, ${FONT_NAME})
	CXXFLAGS := ${CXXFLAGS} -DFONT_NAME=\"${FONT_NAME}\"
endif
# ==============================

all: $(TARGET)

clean:
	@echo "Cleaning makeSimplexTexture"
	@rm -rf $(TARGET) $(MODULES)


$(TARGET): $(MODULES)
	@echo "Linking $(TARGET) ..."
	@$(CXX) -o $(TARGET) $(LDFLAGS) $(MODULES) $(LIBS)

%.o: %.cpp
	@echo "Generating $@ ..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(FFLAGS)

# --- Dependencies ---
callback.cpp: callback.h environment.h consoleui.h
consoleui.cpp: consoleui.h callback.h
environment.cpp: environment.h
main.cpp: main.h workloop.h consoleui.h
renderer.cpp: renderer.h
sfmlui.cpp: sfmlui.h
textbox.cpp: textbox.h
tools.cpp: tools.h
workloop.cpp: workloop.h
wzseq.cpp: wzseq.h
consoleui.h: main.h
environment.h: textbox.h wzseq.h
main.h: tools.h
renderer.h: main.h sfmlui.h
sfmlui.h: main.h
textbox.h: main.h sfmlui.h
workloop.h: main.h renderer.h sfmlui.h

