//
//  REInst.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__REInst__
#define __LexisFontinalis__REInst__

enum {    /* Inst.opcode */
	Char,
	Any,
	Set,
	Match,
	Save,
	Jmp,
	Split
};

struct REInst {
	int opcode;
	int c;
	REInst *x;
	REInst *y;
};


#endif /* defined(__LexisFontinalis__REInst__) */
