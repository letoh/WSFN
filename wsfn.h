#ifndef WSFN_H
#define WSFN_H

#ifndef WSFN_MEM_SIZE
#define WSFN_MEM_SIZE  256
#endif

struct WSFN;
typedef void (*wsfn_action_t)(struct WSFN *);

struct WSFN {
	int acc;
	int x, y, _x, _y;
	int d;
	char *cmds[26];
	char mem[WSFN_MEM_SIZE];
	char *here;
	wsfn_action_t init;
	wsfn_action_t move;
};

void eval(struct WSFN *vm, char *cmdbuf);
void setup(struct WSFN *vm, wsfn_action_t init, wsfn_action_t move);


#endif /* WSFN_H */

