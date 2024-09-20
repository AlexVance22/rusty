BIN = librusty
STD = -std=c++20
CC = g++
LD = ar rcs

SDIR = src
ODIR = obj

INCDIRS = -I$(SDIR) -Iinclude/rusty

CFLAGS = $(STD) -Wall $(INCDIRS)
LFLAGS = $(STD) -Wall

DEPS  = $(wildcard $(SDIR)/*.h)
SRCS  = $(wildcard $(SDIR)/*.cpp)
OBJS  = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))


bin/$(BIN).a: $(OBJS)
	@echo "  linking:" $@
	@$(LD) $@ $^

bin/$(BIN)-r.a: $(OBJS)
	@echo "  linking:" $@
	@$(LD) $@ $^

$(BIN)-test: $(OBJS) obj/main.o
	@echo "  linking:" $@
	@g++ -o $@ $^ $(LFLAGS)
	@echo "  running: ----------"
	@./$(BIN)-test


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@echo "compiling:" $<
	@$(CC) -o $@ -c $< $(CFLAGS)

obj/main.o: test/main.cpp $(DEPS)
	@echo "compiling:" $<
	@$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(OBJS) obj/main.o
	rm -f bin/$(BIN).a
	rm -f bin/$(BIN)-r.a
	rm -f $(BIN)-test


.PHONY: release

release: clean
release: CFLAGS = $(STD) -Wall -O3 $(INCDIRS) -DNDEBUG
release: LFLAGS = $(STD) -Wall -O3
release: bin/$(BIN)-r.a


.PHONY: test

test: clean
test: CFLAGS = $(STD) -Wall $(INCDIRS) -DTESTING
test: $(BIN)-test

