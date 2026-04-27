CC      = gcc
CFLAGS  = -Wall -g -std=gnu99 $(shell pkg-config --cflags gtk+-3.0)
LIBS    = $(shell pkg-config --libs gtk+-3.0) -lm

# GTK4 flags for the GUI build. The GUI sources use GTK4-only APIs
# (gtk_drawing_area_set_draw_func, gtk_widget_add_controller,
# gtk_gesture_click_new, gtk_window_set_child, gdk_texture_new_from_filename,
# gtk_css_provider_load_from_string, etc.), so this target must link
# against GTK4 on the target machine.
GUI_CFLAGS = -Wall -g -std=gnu99 $(shell pkg-config --cflags gtk4)
GUI_LIBS   = $(shell pkg-config --libs gtk4) -lm

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

# ASCII build excludes board.c (GUI-only; depends on Board struct and image code).
ASCII_MODULES = \
    $(SRC_DIR)/ai.c     \
    $(SRC_DIR)/input.c  \
    $(SRC_DIR)/log.c    \
    $(SRC_DIR)/move.c   \
    $(SRC_DIR)/pieces.c \
    $(SRC_DIR)/undo.c

# GUI build: gui.c has its own main(); pulls in board.c + pieces.c for
# board state, and render.c + color_gui.c for drawing.
GUI_MODULES = \
    $(SRC_DIR)/ai.c \
    $(SRC_DIR)/board.c  \
    $(SRC_DIR)/board_bundle.c \
    $(SRC_DIR)/board_state.c \
    $(SRC_DIR)/game_controller.c \
    $(SRC_DIR)/move.c \
    $(SRC_DIR)/pieces.c \
    $(SRC_DIR)/undo.c

GUI_SRCS = \
    $(GUI_DIR)/gui.c        \
    $(GUI_DIR)/render.c     \
    $(GUI_DIR)/color_gui.c

EXE          = $(BIN_DIR)/chess
ASCII_EXE    = $(BIN_DIR)/chess_ascii
GUI_EXE      = $(BIN_DIR)/chess_gui
TEST_RULE    = $(BIN_DIR)/test_rulecheck
TEST_DISPLAY = $(BIN_DIR)/test_boarddisplay

INCLUDES = -I$(SRC_DIR) -I$(GUI_DIR)

# ----- Targets -----

all: $(EXE)

ascii: $(ASCII_EXE)

gui: $(GUI_EXE)

$(EXE): $(SRC_DIR)/main.c $(MODULES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/main.c $(MODULES) $(LIBS)

$(ASCII_EXE): $(SRC_DIR)/main.c $(ASCII_MODULES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ \
	    $(SRC_DIR)/main.c $(ASCII_MODULES) $(ASCII_LIBS)

$(GUI_EXE): $(GUI_SRCS) $(GUI_MODULES) | $(BIN_DIR)
	$(CC) $(GUI_CFLAGS) $(INCLUDES) -o $@ \
	    $(GUI_SRCS) $(GUI_MODULES) $(GUI_LIBS)

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
	rm -rf $(BIN_DIR)/chess $(ASCII_EXE) $(GUI_EXE) $(TEST_RULE) $(TEST_DISPLAY)
	rm -f $(BIN_DIR)/*.log *.log

tar: clean
	cd .. && tar -cvzf Chess_Alpha_src.tar.gz chess

.PHONY: all ascii gui test clean tar
