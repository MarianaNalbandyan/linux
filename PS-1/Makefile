NAME = do-command
FLAGS = -Wall -Wextra -Werror
SRC = main.cpp
OBJ = main.o
GCC = g++

all: $(NAME)

$(NAME) : $(OBJ)
	@ $(GCC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ) : $(SRC)
	@ $(GCC) $(FLAGS) -c $(SRC) -o $(OBJ)

clean: 
	@ rm -rf $(OBJ) $(NAME)

fclean:
	@ rm -f $(OBJ)

