BIN = librusty
STD = -std=c++17
CC = g++
LD = ar rcs

SDIR = src
ODIR = obj

DEFINES =
INCDIRS = -I$(SDIR) -Iinclude/rusty

CFLAGS = $(STD) -Wall $(INCDIRS) $(DEFINES)
LFLAGS = $(STD) -Wall

DEPS = $(wildcard $(SDIR)/*.h)
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))


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


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@echo "compiling:" $<
	@$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
	rm -f $(BIN)-r
	rm -f $(BIN)-test


.PHONY: release

release: clean
release: CFLAGS = $(STD) -Wall -O3 $(INCDIRS) $(DEFINES) -DNDEBUG
release: LFLAGS = $(STD) -Wall -O3
release: $(BIN)-r


.PHONY: test

test: clean
test: CFLAGS = $(STD) -Wall $(INCDIRS) -DTESTING
test: $(BIN)-test


.PHONY: all

all: $(BIN)
all: $(BIN)-r
all: $(BIN)-test

