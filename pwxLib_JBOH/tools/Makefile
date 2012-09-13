.PHONY: all clean

GETRNDIR= src/getRandomName
GRMADIR = src/gravMat
MKSTDIR = src/makeSimplexTexture

all: Makefile
	@echo "Making getRandomName"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" make -j5 -C $(GETRNDIR))
	@echo "Making gravMat"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" FONT_PATH="$(FONT_PATH)" FONT_NAME="$(FONT_NAME)" make -j5 -C $(GRMADIR))
	@echo "Making makeSimplexTexture"
	@(CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" FONT_PATH="$(FONT_PATH)" FONT_NAME="$(FONT_NAME)" make -j5 -C $(MKSTDIR))

clean:
	@echo "Cleaning tools"
	@(make -C $(GETRNDIR) clean)
	@(make -C $(GRMADIR) clean)
	@(make -C $(MKSTDIR) clean)
