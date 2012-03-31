/**
 * quadratic Koch island
 *
 * Dl(RRR)
 * Dp(qRqRqRq)
 * DqT(-qRqlqlqqqRqRqlq+)F
 *
 * Z+++p
 */

void init(void);
void move(int _x, int _y, int x, int y);

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

_D(q);
_D(_L) { _R(); _R(); _R(); }
_D(p) { q(); _R(); q(); _R(); q(); _R(); q(); }
_D(q) { _T(_B(_M(); q(); _R(); q(); _L(); q(); _L(); q(); q(); _R(); q(); _R(); q(); _L(); q(); _P()), _B(_F()) ); }
_D(w) { _Z(); _P(); _P(); _P(); _P(); p(); }

