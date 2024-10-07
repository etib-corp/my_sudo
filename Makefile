NAME = my_sudo

CC = gcc

CFLAGS = -I./include -lcrypt

SRC = $(wildcard src/*.c)

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	sudo chown root:root $(NAME)
	sudo chmod u+s $(NAME)

debug:
	$(CC) $(CFLAGS) -g -o $(NAME) $(SRC)
	sudo chown root:root $(NAME)
	sudo chmod u+s $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
