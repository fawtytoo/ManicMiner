# manic miner

PWD = $(notdir $(shell pwd))
YEAR = $(shell date +'%Y')

TARGET = manicminer

CC = gcc

CFLAGS = -O -MMD
LDFLAGS = -lSDL2

SRC = src
O = linux
$(shell mkdir -p $(O))

OBJS = $(O)/main.o $(O)/system.o $(O)/video.o $(O)/loader.o $(O)/title.o $(O)/audio.o $(O)/miner.o $(O)/levels.o $(O)/game.o $(O)/portal.o $(O)/trans.o $(O)/gameover.o $(O)/robots.o $(O)/victory.o $(O)/cheat.o $(O)/die.o $(O)/spg.o $(O)/misc.o

all:	$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(O)/loader.o:	$(SRC)/loader.c
	$(CC) $(CFLAGS) -DBUILD=\"$(PWD).$(YEAR)\" -c $< -o $@

$(O)/%.o:	$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(O) $(TARGET)

install: all
	cp $(TARGET) ~/.local/bin/

uninstall:
	rm ~/.local/bin/$(TARGET)

-include $(O)/*.d

# manic miner
