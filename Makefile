CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

# Main object files without compiled assembler functions.o
OBJ = integral.o calculation.o solution.o testing.o
TEST_OBJ = testintegral.o calculation.o solution.o testing.o

ifeq ($(OS),Windows_NT)
	ASM_FORMAT = win32
	PREFIX_FLAG =
	RM = del /f /q
	EXE = .exe
else
	ASM_FORMAT = elf32
	PREFIX_FLAG =
	RM = rm -f
	EXE =
endif

ifeq ($(SPEC_FILE),)
$(warning PLEASE SET SPEC_FILE ENV VARIABLE)
endif


.PHONY: all test clean

all: integral$(EXE) testintegral$(EXE)

integral$(EXE): $(OBJ) functions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

testintegral$(EXE): $(TEST_OBJ) functions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

functions.o: functions.asm
	nasm -f $(ASM_FORMAT) $(PREFIX_FLAG) functions.asm -o functions.o

# Generating functions.asm via builder. Run with "SPEC_FILE=in.txt make"
functions.asm: $(SPEC_FILE)
	$(MAKE) -C builder building$(EXE)
	builder/building$(EXE) -o -r > functions.tmp
	mv functions.tmp functions.asm


test: testintegral$(EXE)
	./testintegral$(EXE) --test-root 1:2:-1.5:1.5:0.00001:1.34365 \
	           --test-root 1:3:-1.5:1.5:0.00001:-1.30786 \
	           --test-root 2:3:-1.5:1.5:0.00001:0.82622
	./testintegral$(EXE) --test-integral 1:-1.5:1.5:0.0001:10.86228 \
		   --test-integral 2:-2.0:2.0:0.0001:0.0 \
		   --test-integral 3:1.0:6.0:0.0001:0.69883
	./testintegral$(EXE) --test-integral 2:0.0:15.0:0.0001:12656.25


clean:
	$(RM) *.o integral$(EXE) testintegral$(EXE)
