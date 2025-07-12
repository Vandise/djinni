CC          := gcc
ARCHIVER    := ar
INC_DIR     := inc
ADDFLAGS    := -g -O2 -std=c11
SYSINCDIR := /usr/include

LIBRARIES := lib
SOURCES   := src
TESTS     := test
OBJECTS   := obj
LIBNAME   := libdjinni.a

ENGINE_MODULE_FILES := $(shell find $(SOURCES) -type f -name *.c)
ENGINE_OBJECTS      := $(patsubst $(SOURCES)/%.c, $(OBJECTS)/%.o, $(ENGINE_MODULE_FILES))

PROGRAM_FILES        := $(shell find $(TESTS) -type f -name "*.c")
PROGRAM_OBJECTS      := $(patsubst $(TESTS)/%.c, obj/%.o, $(PROGRAM_FILES))

all: clean engine program

program: $(PROGRAM_OBJECTS)
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I inc/ -o bin/test $(PROGRAM_OBJECTS) $(LIBRARIES)/$(LIBNAME) -lsdl2 -lsdl2_mixer -lsdl2_ttf -lsdl2_image -lm

$(OBJECTS)/%.o: $(TESTS)/%.c
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I inc/ -c $< -o $@

engine: $(ENGINE_OBJECTS)
	$(ARCHIVER) rcs $(LIBRARIES)/$(LIBNAME) $(ENGINE_OBJECTS)

$(OBJECTS)/%.o: $(SOURCES)/%.c
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I $(INC_DIR) -c $< -o $@

clean:
	rm -f bin/test
	rm -rf bin/test.dSYM
	rm -f $(LIBRARIES)/$(LIBNAME)
	rm -f $(shell find obj -type f -name "*.o")