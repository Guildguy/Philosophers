CC = cc
FLAGS = -pthread #-fsanitize=thread //dispara aviso que está ocorrendo uma condição do corrida a var global

SRC_DIR = src/
OBJ_DIR = obj/

SRC = $(SRC_DIR)/philosopher_creation.c $(SRC_DIR)/data_handler.c $(SRC_DIR)/philo.c
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

HEADER = philo.h
EXEC = philo

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

.PHONY: all clean fclean re