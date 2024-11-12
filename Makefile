PROJECT_DIR=${PWD}
CC_FLAGS= -Wall -Wextra -Werror

.PHONY: clean remove

$(PROJECT_DIR)/main.out: $(PROJECT_DIR)/main.o
	gcc $(PROJECT_DIR)/main.o -o $(PROJECT_DIR)/main.out $(CC_FLAGS)

$(PROJECT_DIR)/main.o: $(PROJECT_DIR)/src/main.c
	gcc -c $(PROJECT_DIR)/src/main.c -o $(PROJECT_DIR)/main.o $(CC_FLAGS)

clean:
	rm $(PROJECT_DIR)/*.o

remove:
	rm $(PROJECT_DIR)/main.out
