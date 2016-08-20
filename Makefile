CC?=gcc
CXX?=g++

PREFIX?=/usr

CFLAGS=-Wall -Wextra -O2 -std=c11 `llvm-config --cflags`
LDFLAGS=`llvm-config --cxxflags --ldflags`

CFILES=$(wildcard src/*.c)
OBJFILES=$(patsubst %.c,%.o, $(CFILES))
LIBFILES=$(filter-out src/main.o, $(OBJFILES))

TESTFILES=$(wildcard tests/*_test.c)
TESTOBJS=$(patsubst %.c,%, $(TESTFILES))

all: build build/erupt

build/erupt: $(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $^

build: $(OBJFILES)
	@mkdir -p build

install: all
	install -Dm755 build/erupt $(PREFIX)/bin/erupt

clean:
	@rm -rf $(OBJFILES) build/

test: build $(TESTOBJS)
	@-./tests/runall.sh

$(TESTOBJS): %: %.c $(TESTFILES) build/tests
	@$(CC) $(CFLAGS) $(LIBFILES) -lrt -lm -Isrc -o build/$@ $<

build/tests:
	@mkdir -p build/tests

.PHONY: install clean test build
