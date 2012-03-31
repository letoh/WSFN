#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include "SDL_draw.h"
#include <setjmp.h>

void move(int, int, int, int);

int acc = 8;
int _x, x;
int _y, y;
int dd[] = { /* up */ 0x01, /* right */ 0x10, /* down */ 0x08, /* left */ 0x80 };
int d; /**< direction, 0: up, 2: down, 1: right, 3: left */

void _M(void) { if(acc) --acc; }
void _P(void) { ++acc; }
void _Z(void) { x = y = 0; }
void _R(void) { d = ++d % 4; }
void _F(void) { _x = x; _y = y;
	x += ((-1 * !!(dd[d] & 0x80)) + !!(dd[d] & 0x10));
	y += ((-1 * !!(dd[d] & 0x08)) + (dd[d] & 0x01));
	move(_x, _y, x, y); }
#define _T(a, b)  if(acc && a) { return; } b
#define _B(...)  ({__VA_ARGS__; 1;})
#define _D(n) void n(void)

_D(q); /* forward declaration */
_D(p) { _T( _B(_M(); p(); _R(); q(); _P()), _B(_F()) ); }
_D(q) { _T( _B(_M(); p(); _R(); _R(); _R(); q(); _P()), _B(_F()) ); }
_D(v) { _Z(); p(); }

/* SDL backend */

#define WIDTH  900
#define HEIGHT 900
#define ORIG_X  (WIDTH / 2)
#define ORIG_Y  (HEIGHT / 2)
#define SCALE   16
#define XX(x)  (ORIG_X + (x) * SCALE)
#define YY(y)  (ORIG_Y - (y) * SCALE)
#define INTERVAL 1

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

void move(int _x, int _y, int x, int y)
{
	Uint32 _x1 = XX(_x), _y1 = YY(_y), x1 = XX(x), y1 = YY(y);
	if(_x1 < 0 || _y1 < 0 || x1 < 0 || y1 < 0 ||
		_x1 >= WIDTH || _y1 >= HEIGHT || x1 >= WIDTH || y1 >= HEIGHT) return;
	Draw_Line(screen, _x1, _y1, x1, y1, SDL_MapRGB(screen->format, 0xfa, 0x3f, 0xa0));
	SDL_Flip(screen);
	delay();
}

int draw_func(void *arg)
{
	if(!setjmp(env)) v();
	return 0;
}

SDL_Surface * InitCanvas(void)
{
	SDL_Surface *screen;

	if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ||
		NULL == (screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF)))
		return NULL;
	SDL_WM_SetCaption("SDL Test", 0);
	atexit(SDL_Quit);

	/* draw background */
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 25, 45, 60) );
	/* draw axis */
	Draw_Line(screen, 0, ORIG_Y, WIDTH-1, ORIG_Y,
			SDL_MapRGB(screen->format, 0x20, 0x50, 0x40) );
	Draw_Line(screen, ORIG_X, 0, ORIG_X, HEIGHT-1,
			SDL_MapRGB(screen->format, 0x20, 0x50, 0x40) );
	SDL_Flip(screen);
	return screen;
}

int main(int argc, char *argv[])
{
	SDL_Thread *drawer;
	SDL_Event event;

	if(NULL == (screen = InitCanvas()) ) return 1;
	drawer = SDL_CreateThread(draw_func, NULL);
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


