CC          := gcc
ARCHIVER    := ar
INC_DIR     := inc
ADDFLAGS    := -g -O2 -std=c11
SYSINCDIR := /usr/include

LIBRARIES := lib
SOURCES   := src
OBJECTS   := obj
LIBNAME   := libdjinni.a

ENGINE_MODULE_FILES := $(shell find $(SOURCES) -type f -name *.c)
ENGINE_OBJECTS      := $(patsubst $(SOURCES)/%.c, $(OBJECTS)/%.o, $(ENGINE_MODULE_FILES))

all: engine program

program:
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I inc/ test/main.c -o bin/test $(LIBRARIES)/$(LIBNAME) -lsdl2 -lsdl2_image -lm

engine: $(ENGINE_OBJECTS)
	$(ARCHIVER) rcs $(LIBRARIES)/$(LIBNAME) $(ENGINE_OBJECTS)

$(OBJECTS)/%.o: $(SOURCES)/%.c
	$(CC) $(ADDFLAGS) -I $(SYSINCDIR) -I $(INC_DIR) -c $< -o $@

clean:
	rm -f bin/test
	rm -rf bin/test.dSYM
	rm -f $(LIBRARIES)/$(LIBNAME)
	rm -f $(shell find obj -type f -name "*.o")