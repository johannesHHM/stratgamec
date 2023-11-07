SRC_DIR := src
OBJ_DIR := build/obj
BIN_DIR := build

EXE := $(BIN_DIR)/game
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall 
LDFLAGS  := -Llib -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
LDLIBS   := -lm

.PHONY: all clean run mem_check

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

run: $(EXE)
	$(EXE)

mem_check: $(EXE)
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			$(EXE)


-include $(OBJ:.o=.d)
