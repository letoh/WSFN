/**
 * WSFN ("Which Stands for Nothing") 
 */
#include <stdio.h>
#include <string.h>
#include "wsfn.h"

#define PROG    "WSFN"
#define BANNER  "Welcome to " PROG "'s World (" __DATE__ ")\n\n"
#define PROMPT  PROG "> "

static void init(struct WSFN *vm)
{
	printf("init\n");
}

static void move(struct WSFN *vm)
{
	printf("move [%d,%d]-[%d,%d]\n", vm->_x, vm->_y, vm->x, vm->y);
}

static char *input(const char *prompt, char *buf, int size, FILE *in)
{
	char *p;
#ifndef SILENT
	printf(prompt);
#endif
	p = fgets(buf, size, in);
	if(!p || feof(in)) return NULL;
	if( (p = strchr(buf, '\n')) ) *p = '\0';
	return buf;
}

int main(int argc, char *argv[])
{
	char buf[128];
	struct WSFN wsfn = {0};
#ifndef SILENT
	printf(BANNER);
#endif
	setup(&wsfn, &init, &move);
	while(input(PROMPT, buf, sizeof(buf), stdin) )
		eval(&wsfn, buf);
	return 0;
}

