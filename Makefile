RANLIB = ranlib

CFLAGS  += -Os -s
LDFLAGS += -L. -lwsfn -s

CAIRO_INC = `pkg-config --cflags cairo`
CAIRO_LIB = `pkg-config --libs cairo`

SDL_INC = `sdl-config --cflags` -Ilib
SDL_LIB = `sdl-config --libs` -Llib -lSDL_draw

CAIRO = 
SDL = shell_sdl
WSFN = libwsfn.a

all: $(WSFN) shell sdl

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

libwsfn.a: wsfn.o
	$(AR) rc $@ $<
	$(RANLIB) $@

shell: shell.o $(WSFN)
	$(CC) -o $@ $< $(LDFLAGS)

cairo:
	CFLAGS="$(CAIRO_INC)" LDFLAGS="$(CAIRO_LIB)" $(MAKE) $(CAIRO)

sdl:
	CFLAGS="$(SDL_INC)" LDFLAGS="$(SDL_LIB)" $(MAKE) $(SDL)

clean:
	rm -f $(CAIRO) $(SDL) *.o $(WSFN) shell

