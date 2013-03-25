CC = gcc
CFLAGS = -DGLEW_STATIC -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99
LDFLAGS = -lglfw -lGLEW -lGL -lalut -lm 

SRCS = bootstrap.c shader.c bitmap.c tile.c game.c
OBJS = $(SRCS:.c=.o)

BIN = space_training

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(BIN) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(BIN)
