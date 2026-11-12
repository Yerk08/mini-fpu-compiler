CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

# using simple compilation rules (no -m32 and -fno-pie)
CC +=

LDLIBS = -lm

OBJ = astree.o building.o generator.o parser.o derivs.o optimizations.o debugging.o

ifeq ($(OS),Windows_NT)
    RM = del /f /q
    EXE = .exe
else
    RM = rm -f
    EXE =
endif


.PHONY: all test clean

all: building$(EXE)

building$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<


test: building$(EXE)
	@echo "+++ Should be an error:"
	-@SPEC_FILE=nothing ./building$(EXE)
	@echo "------------------------------------------------------"
	@echo "+++ Should return all allocation and free correctness:"
	-@SPEC_FILE=test.txt valgrind ./building$(EXE) -o >/dev/zero
	@echo "------------------------------------------------------"
	@echo "+++ Should be large dump of entire system:"
	@SPEC_FILE=test.txt ./building$(EXE) -t >/dev/zero
	@echo "------------------------------------------------------"
	@echo "+++ Should be optimized dump with fsubrp and fdivrp funcs:"
	@SPEC_FILE=test.txt ./building$(EXE) -t -r -o


clean:
	$(RM) *.o building$(EXE)
