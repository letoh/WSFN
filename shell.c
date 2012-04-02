/**
 * WSFN ("Which Stands for Nothing") 
 */
#include <stdio.h>
#include <string.h>

#define PROG    "WSFN"
#define BANNER  "Welcome to " PROG "'s World (" __DATE__ ")\n\n"
#define PROMPT  PROG "> "

struct WSFN {
	int acc;
	int x, y, _x, _y;
	int d;
	char *cmds[26];
	char mem[256];
	char *here;
};
static int dd[] = { /* up */ 0x01, /* right */ 0x10, /* down */ 0x08, /* left */ 0x80 };

static char * skipcmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	//printf("\tskip '%c'\n", *p);
	switch(*p | 0x20)
	{
	case '(':
		while(*++p && *p != ')')
			p = skipcmd(vm, p);
		break;
	//case 'z': case '-': case '+': case 'r': case 'f':
	defualt: /* user defined */
		break;
	}
	return p;
}

static char * storecmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	printf("\t\tstore %c\n", *p);
	*vm->here++ = *p;
	switch(*p | 0x20)
	{
	case '(':
		while(*++p && *p != ')')
			p = storecmd(vm, p);
		*vm->here++ = *p;
		break;
	case 't':
		p = storecmd(vm, p + 1);
		p = storecmd(vm, p + 1);
		break;
	case 'd':
	default:
		break;
	}
	return p;
}

static char * defcmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	int idx = *p - 'a';
	printf("\tdef %c, idx: %d\n", *p, idx);
	vm->cmds[*p - 'a'] = vm->here;
	p = storecmd(vm, p + 1);
	++vm->here;
	printf("\tdef %c '%s'\n", 'a' + idx, vm->cmds[idx]);
	return p;
}

static char * execcmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	char *q;
	printf("\tcmd '%c'\n", *p);
	switch(*p | 0x20)
	{
	case '.':
		printf("acc: %d (%d, %d)\n", vm->acc, vm->x, vm->y);
		break;
	case 'z': /* init_canvas() */
		vm->x = vm->y = 0;
		break;
	case '-': if(vm->acc) --vm->acc; break;
	case '+': ++vm->acc; break;
	case 'r': vm->d = ++vm->d % 4; break;
	case 'f': vm->_x = vm->x; vm->_y = vm->y;
		vm->x += ((-1 * !!(dd[vm->d] & 0x80)) + !!(dd[vm->d] & 0x10));
		vm->y += ((-1 * !!(dd[vm->d] & 0x08)) + (dd[vm->d] & 0x01));
		/* move(_x, _y, x, y); */
		break;
	case '(':
		while(*++p && *p != ')')
			p = execcmd(vm, p);
		break;
	case 't':
		if(!vm->acc)
			p = execcmd(vm, skipcmd(vm, p + 1) + 1);
		else
			p = skipcmd(vm, execcmd(vm, p + 1) + 1);
		break;
	case 'd':
		p = defcmd(vm, p + 1);
		break;
	default: /* user defined */
		if( (q = vm->cmds[*p - 'a']) )
			while(*q)
				q = execcmd(vm, q) + 1;
		break;
	}
	return p;
}

void eval(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	printf("eval '%s'\n", cmdbuf);
	while(*p)
	{
		switch(*p)
		{
		case ' ': case '\t': break;
		default:
			p = execcmd(vm, p);
			break;
		}
		++p;
	}
}

char *input(const char *prompt, char *buf, int size, FILE *in)
{
	char *p;
	printf(prompt);
	p = fgets(buf, size, in);
	if(!p || feof(in)) return NULL;
	if( (p = strchr(buf, '\n')) ) *p = '\0';
	return buf;
}

int main(int argc, char *argv[])
{
	char buf[128];
	struct WSFN wsfn = {0};
	wsfn.here = wsfn.mem;
	printf(BANNER);
	while(input(PROMPT, buf, sizeof(buf), stdin) )
		eval(&wsfn, buf);
	return 0;
}

