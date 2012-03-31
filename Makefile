CAIRO_INC = `pkg-config --cflags cairo`
CAIRO_LIB = `pkg-config --libs cairo`

SDL_INC = `sdl-config --cflags` -Ilib
SDL_LIB = `sdl-config --libs` -Llib -lSDL_draw

CAIRO = dragon_cairo b
SDL = dragon_sdl

all: cairo sdl

cairo:
	CFLAGS="$(CAIRO_INC)" LDFLAGS="$(CAIRO_LIB)" $(MAKE) $(CAIRO)

sdl:
	CFLAGS="$(SDL_INC)" LDFLAGS="$(SDL_LIB)" $(MAKE) $(SDL)

clean:
	rm -f $(CAIRO) $(SDL)

