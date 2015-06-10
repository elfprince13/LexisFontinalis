//
//  RepeaterNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__RepeaterNode__
#define __LexisFontinalis__RepeaterNode__

#include "OperationNode.h"

class RepeaterNode : public OperationNode {
public:
	char min;
	char max;
	bool greedy;
	RepeaterNode(char min, char max, bool greedy, RENode* constructAround) :
	OperationNode(Repeat, Left, 1), min(min), max(max), greedy(greedy) {
		children.push_back(constructAround);
	}
	
	~RepeaterNode() {}

	virtual REInst * compile();
};

#endif /* defined(__LexisFontinalis__RepeaterNode__) */
