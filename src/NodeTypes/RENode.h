//
//  RENode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__RENode__
#define __LexisFontinalis__RENode__

//#include <iostream>
#include <vector>
#include <functional>

#include "../REISA/REInst.h"

class RENode {
public:
	
	virtual REInst * compile() = 0;
	
	virtual ~RENode() {}
	
	/*
	 enum REOps {
		Concatenation,
		Alternation
		Repeat,
		Group,
		Embed,
		Range,
		Wildcard,
		Literal,
	};
	 */
};


/*

class RENodeFactory {
private:
	bool usable;
	std::function<RENode*(RENode*)> factoryFunc;
	RENodeFactory (std::function<RENode*(RENode*)> const & fF) : usable(true), factoryFunc(fF) {};
public:
	RENodeFactory () : usable(false), factoryFunc() {};
	bool isUsable() { return usable; }
	
	RENode * operator() (RENode * constructAround);
	RENodeFactory operator() (RENodeFactory composeWith);
	
	static RENodeFactory makeErrorFactory(std::string errContent);
	static RENodeFactory makeRepeatFactory(char min, char max, bool greedy);
}; */

#endif /* defined(__LexisFontinalis__RENode__) */
