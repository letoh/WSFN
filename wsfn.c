#include "wsfn.h"

static int dd[] = { /* up */ 0x01, /* right */ 0x10, /* down */ 0x08, /* left */ 0x80 };

static char * skipcmd(struct WSFN *vm, char *cmdbuf)
{
	if(*cmdbuf == '(')
		while(*++cmdbuf && *cmdbuf != ')')
			cmdbuf = skipcmd(vm, cmdbuf);
	return cmdbuf;
}

static char * storecmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
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
	default:
		break;
	}
	return p;
}

static char * defcmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	vm->cmds[*p - 'a'] = vm->here;
	p = storecmd(vm, p + 1);
	++vm->here;
	return p;
}

static char * execcmd(struct WSFN *vm, char *cmdbuf)
{
	char *p = cmdbuf;
	char *q;
	switch(*p | 0x20)
	{
	case 'z':
		vm->x = vm->y = 0;
		if(vm->init) vm->init(vm);
		break;
	case '-': if(vm->acc) --vm->acc; break;
	case '+': ++vm->acc; break;
	case 'r': vm->d = ++vm->d % 4; break;
	case 'f': vm->_x = vm->x; vm->_y = vm->y;
		vm->x += ((-1 * !!(dd[vm->d] & 0x80)) + !!(dd[vm->d] & 0x10));
		vm->y += ((-1 * !!(dd[vm->d] & 0x08)) + (dd[vm->d] & 0x01));
		if(vm->move) vm->move(vm);
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

void setup(struct WSFN *vm, wsfn_action_t init, wsfn_action_t move)
{
	vm->here = vm->mem;
	vm->init = init;
	vm->move = move;
}

