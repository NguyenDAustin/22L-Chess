CC      = gcc
CFLAGS  = -Wall -g -std=gnu99 $(shell pkg-config --cflags gtk4)
LIBS    = $(shell pkg-config --libs gtk4) -lm

SRC_DIR = src
GUI_DIR = src/gui
BIN_DIR = bin

# Module sources (everything except main and test drivers)
MODULES = \
    $(SRC_DIR)/ai.c     \
    $(SRC_DIR)/board.c  \
    $(SRC_DIR)/input.c  \
    $(SRC_DIR)/log.c    \
    $(SRC_DIR)/move.c   \
    $(SRC_DIR)/pieces.c \
    $(SRC_DIR)/undo.c

GUI_SRCS = \
    $(GUI_DIR)/gui.c        \
    $(GUI_DIR)/render.c     \
    $(GUI_DIR)/color_gui.c

EXE          = $(BIN_DIR)/chess
TEST_RULE    = $(BIN_DIR)/test_rulecheck
TEST_DISPLAY = $(BIN_DIR)/test_boarddisplay

INCLUDES = -I$(SRC_DIR) -I$(GUI_DIR)

# ----- Targets -----

all: $(EXE)

$(EXE): $(SRC_DIR)/main.c $(MODULES) $(GUI_SRCS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/main.c $(MODULES) $(GUI_SRCS) $(LIBS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ----- Tests -----

test: $(TEST_RULE) $(TEST_DISPLAY)
	@echo "--- running test_rulecheck ---"
	./$(TEST_RULE)
	@echo "--- running test_boarddisplay ---"
	./$(TEST_DISPLAY)

$(TEST_RULE): $(SRC_DIR)/test_rulecheck.c $(SRC_DIR)/pieces.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/test_rulecheck.c $(SRC_DIR)/pieces.c $(LIBS)

$(TEST_DISPLAY): $(SRC_DIR)/test_boarddisplay.c $(SRC_DIR)/pieces.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/test_boarddisplay.c $(SRC_DIR)/pieces.c $(LIBS)

# ----- Housekeeping -----

clean:
	rm -rf $(BIN_DIR)/chess $(TEST_RULE) $(TEST_DISPLAY)
	rm -f $(BIN_DIR)/*.log *.log

tar: clean
	cd .. && tar -cvzf Chess_Alpha_src.tar.gz chess

.PHONY: all test clean tar
