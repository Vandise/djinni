CC          := gcc
ARCHIVER    := ar
INC_DIR     := inc
ADDFLAGS    := -g -O0 -std=c11
SYSINCDIR := /usr/include

LIBRARIES := lib
SOURCES   := src
DEV       := dev
OBJECTS   := obj
LIBNAME   := libdjinni.a

ENGINE_MODULE_FILES := $(shell find $(SOURCES) -type f -name *.c)
ENGINE_OBJECTS      := $(patsubst $(SOURCES)/%.c, $(OBJECTS)/%.o, $(ENGINE_MODULE_FILES))

PROGRAM_FILES        := $(shell find $(DEV)/$(example) -type f -name "*.c")
PROGRAM_OBJECTS      := $(patsubst $(DEV)/$(example)/%.c, obj/$(example)/%.o, $(PROGRAM_FILES))

all: clean engine

program: $(PROGRAM_OBJECTS)
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I inc/ -o bin/example $(PROGRAM_OBJECTS) $(LIBRARIES)/$(LIBNAME) -lsdl2 -lsdl2_mixer -lsdl2_ttf -lsdl2_image -lm

$(OBJECTS)/$(example)/%.o: $(DEV)/$(example)/%.c
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I $(DEV)/$(example)/inc -I inc/ -c $< -o $@

engine: $(ENGINE_OBJECTS)
	$(ARCHIVER) rcs $(LIBRARIES)/$(LIBNAME) $(ENGINE_OBJECTS)

$(OBJECTS)/%.o: $(SOURCES)/%.c
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I $(INC_DIR) -c $< -o $@

clean:
	rm -f bin/test
	rm -rf bin/test.dSYM
	rm -f bin/example
	rm -rf bin/example.dSYM
	rm -f $(LIBRARIES)/$(LIBNAME)
	rm -f $(shell find obj -type f -name "*.o")