BIN = librusty
EXT = cpp
STD = -std=c++17
CC = g++
LD = ar rcs

SDIR = src
ODIR = obj

DEFS =
LIBS =
DLNKS =
RLNKS =

DEFINES = $(patsubst %, -D %, $(DEFS))
INCDIRS = $(patsubst %, -Ilib/%/include, $(LIBS)) -I$(SDIR) -Iinclude/rusty
LIBDIRS = $(patsubst %, -Llib/%/bin, $(LIBS))
DLINKS = $(patsubst %, -l%, $(DLNKS))
RLINKS = $(patsubst %, -l%, $(RLNKS))

CFLAGS = $(INCDIRS) $(DEFINES) $(STD) -Wall
LFLAGS = $(LIBDIRS) $(DLINKS) $(STD) -Wall

DEPS = $(wildcard $(SDIR)/*.h)
SRCS = $(wildcard $(SDIR)/*.$(EXT))
OBJS = $(patsubst $(SDIR)/%.$(EXT), $(ODIR)/%.o, $(SRCS))


bin/$(BIN).a: $(OBJS)
	@echo "  linking:" $@
	@$(LD) $@ $^

bin/$(BIN)-r.a: $(OBJS)
	@echo "  linking:" $@
	@$(LD) $@ $^

$(BIN)-test: $(OBJS)
	@echo "  linking:" $@
	@g++ -o $@ $^ $(LFLAGS)
	@echo "  running: ----------"
	@./$(BIN)-test


$(ODIR)/%.o: $(SDIR)/%.$(EXT) $(DEPS)
	@echo "compiling:" $<
	@$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
	rm -f $(BIN)-r


.PHONY: all

all: $(BIN)


.PHONY: release

release: CFLAGS = $(INCDIRS)  -DNDEBUG $(STD) -Wall -O2
release: LFLAGS = $(LIBDIRS) $(RLINKS) $(STD) -Wall -O2
release: clean
release: $(BIN)-r


.PHONY: test

test: CFLAGS = $(INCDIRS) -DNDEBUG -DTESTING $(STD) -Wall
test: $(BIN)-test


.PHONY: run

run: $(BIN)
	@echo "  running: ----------"
	@./$(BIN)

