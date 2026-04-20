CC      = gcc
CFLAGS  = -Wall -g -std=gnu99 $(shell pkg-config --cflags gtk+-3.0)
LIBS    = $(shell pkg-config --libs gtk+-3.0) -lm

# ASCII build: pieces.h still pulls in <gtk/gtk.h> for the cairo_surface_t
# typedef, so we need the GTK cflags, but main.c never calls into GTK/cairo,
# so we can skip linking the heavy GUI libs.
ASCII_LIBS = -lm

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
ASCII_EXE    = $(BIN_DIR)/chess_ascii
TEST_RULE    = $(BIN_DIR)/test_rulecheck
TEST_DISPLAY = $(BIN_DIR)/test_boarddisplay

INCLUDES = -I$(SRC_DIR) -I$(GUI_DIR)

# ----- Targets -----

all: $(EXE)

ascii: $(ASCII_EXE)

$(EXE): $(SRC_DIR)/main.c $(MODULES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/main.c $(MODULES) $(LIBS)

$(ASCII_EXE): $(SRC_DIR)/main.c $(MODULES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/main.c $(MODULES) $(ASCII_LIBS)

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
	rm -rf $(BIN_DIR)/chess $(ASCII_EXE) $(TEST_RULE) $(TEST_DISPLAY)
	rm -f $(BIN_DIR)/*.log *.log

tar: clean
	cd .. && tar -cvzf Chess_Alpha_src.tar.gz chess

.PHONY: all ascii test clean tar
