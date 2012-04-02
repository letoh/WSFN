#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include "SDL_draw.h"
#include <setjmp.h>

#include "wsfn.h"

/* SDL backend */

#define WIDTH  500
#define HEIGHT 500
#define ORIG_X  (WIDTH / 2)
#define ORIG_Y  (HEIGHT / 2)
#define SCALE   10
#define XX(x)  (ORIG_X + (x) * SCALE)
#define YY(y)  (ORIG_Y - (y) * SCALE)
#define INTERVAL 3

int done;
SDL_Surface* screen;
jmp_buf env;

void delay(void)
{
	static Uint32 next;
	Uint32 now;
	while((now = SDL_GetTicks() ) <= next)
	{
		if(done) longjmp(env, 1);
		SDL_Delay(1);
	}
	next = now + INTERVAL;
}

static void init(struct WSFN *vm)
{
	/* draw background */
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 25, 45, 60) );
	/* draw axis */
	Draw_Line(screen, 0, ORIG_Y, WIDTH - 1, ORIG_Y,
			SDL_MapRGB(screen->format, 0x20, 0x50, 0x40) );
	Draw_Line(screen, ORIG_X, 0, ORIG_X, HEIGHT - 1,
			SDL_MapRGB(screen->format, 0x20, 0x50, 0x40) );
	SDL_Flip(screen);
}

static void move(struct WSFN *vm)
{
	Uint32 _x1 = XX(vm->_x), _y1 = YY(vm->_y), x1 = XX(vm->x), y1 = YY(vm->y);
	if(_x1 < 0 || _y1 < 0 || x1 < 0 || y1 < 0 ||
		_x1 >= WIDTH || _y1 >= HEIGHT || x1 >= WIDTH || y1 >= HEIGHT) return;
	Draw_Line(screen, _x1, _y1, x1, y1, SDL_MapRGB(screen->format, 0xfa, 0x3f, 0xa0));
	SDL_Flip(screen);
	delay();
}

int draw_func(void *arg)
{
	if(!setjmp(env)) eval((struct WSFN *)arg, "DpT(-pRq+)F DqT(-pRRRq+)F Z+++++++p");
	return 0;
}

SDL_Surface * InitCanvas(void)
{
	if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ||
		NULL == (screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF)))
		return NULL;
	SDL_WM_SetCaption("WSFN (SDL)", 0);
	atexit(SDL_Quit);
	init(NULL);
	return screen;
}

int main(int argc, char *argv[])
{
	SDL_Thread *drawer;
	SDL_Event event;
	struct WSFN wsfn = {0};

	setup(&wsfn, init, move);

	if(NULL == InitCanvas() ) return 1;
	drawer = SDL_CreateThread(draw_func, (void *)&wsfn);
	while (!done)
	{
		if (SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN &&
				 (event.key.keysym.sym == SDLK_ESCAPE ||
				  event.key.keysym.sym == SDLK_q)) )
				done = 1;
		}
		SDL_Delay(1);
	}
	SDL_WaitThread(drawer, NULL);
	return 0;
}

