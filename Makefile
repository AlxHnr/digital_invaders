P_NAME  = digital_invaders
OBJECTS = main scrolling_tile framerate graphic bullet spaceship enemy collision \
	  scrolling_text weapon powerup spaceship_enchantments game interface ai \
	  gui_engine gui_theme developers_panel

# flags
CFLAGS  = -Wall -Wextra -O2 `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lm -lSDL_gfx -lSDL_image -lSDL_ttf

# build OBJECTS
.PHONY: all
all:   $(P_NAME)/$(P_NAME)

# build main program
$(P_NAME)/$(P_NAME): $(OBJECTS:%=obj/%.o) $(P_NAME)
	$(CC) $(LDFLAGS) $(OBJECTS:%=obj/%.o) -o $(P_NAME)/$(P_NAME)

# create main directory
$(P_NAME):
	mkdir $(P_NAME)

# compile objects
obj/%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# additional stuff
.PHONY: run
run: $(P_NAME)/$(P_NAME)
	cd $(P_NAME) && ./$(P_NAME)

clean:
	- rm -rf $(P_NAME)/$(P_NAME) obj
	- rmdir --ignore-fail-on-non-empty $(P_NAME)
