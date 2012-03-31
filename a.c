/**
 * FX
 * X=X+YF+
 * Y=-FX-Y
 *
 * DpT(-pRq+)F
 * DqT(-pRRRq+)F
 *
 * Z++++++++p
 */
#include <stdio.h>
#include <cairo.h>

cairo_surface_t *surface;
cairo_t *cr;

#define WIDTH  400
#define HEIGHT 400
#define SCALE  9.0

void init(void)
{
	if(!surface)
		surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, WIDTH, HEIGHT);
	if(!cr)
		cr = cairo_create(surface);

	/* white background */
	cairo_set_source_rgb(cr, 1., 1., 1.);
	cairo_rectangle(cr, 0., 0., WIDTH, HEIGHT);
	cairo_fill(cr);

    cairo_set_line_width(cr, .1);
	//cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	cairo_save(cr);
		cairo_set_source_rgb(cr, 1., 0., 0.);
		cairo_move_to(cr, 0, HEIGHT/2); cairo_line_to(cr, WIDTH, HEIGHT/2);
		cairo_stroke(cr);
		cairo_set_source_rgb(cr, 0., 1., 0.);
		cairo_move_to(cr, WIDTH/2, 0); cairo_line_to(cr, WIDTH/2, HEIGHT);
		cairo_stroke(cr);
	cairo_restore(cr);

    cairo_set_source_rgb(cr, 0., 0., 0.);
	cairo_translate(cr, WIDTH/2, HEIGHT/2);
	cairo_move_to(cr, 0, 0);
	cairo_scale(cr, SCALE, -SCALE);
}

void move(int _x, int _y, int x, int y)
{
	//printf("[x, y] = (%d, %d) -> (%d, %d)\n", _x, _y, x, y);
	cairo_move_to(cr, _x, _y);
	cairo_line_to(cr, x, y);
	//cairo_rel_line_to(cr, x, y);
	cairo_stroke(cr);
}

void finish(void)
{
	if(!surface || !cr) return;
	cairo_surface_write_to_png(surface, "a.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

int acc;
int _x, x;
int _y, y;
int dd[] = { /* up */ 0x01, /* right */ 0x10, /* down */ 0x08, /* left */ 0x80 };
int d; /**< direction, 0: up, 2: down, 1: right, 3: left */

void _M(void) { if(acc) --acc; }
void _P(void) { ++acc; }
void _Z(void) { init(); x = y = 0; }
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

int main(int argc, char *argv[])
{
	atexit(finish);
	_Z(); _P(); _P(); _P(); _P(); _P(); _P(); _P(); _P(); p();
	return 0;
}

