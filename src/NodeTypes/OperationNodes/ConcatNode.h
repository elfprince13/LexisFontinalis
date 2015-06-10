//
//  ConcatNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__ConcatNode__
#define __LexisFontinalis__ConcatNode__

#include "OperationNode.h"

class ConcatNode : public OperationNode {
public:
	ConcatNode(RENode * first, RENode * second) :
	OperationNode(Concat, Left, 2) {
		children.push_back(first);
		children.push_back(second);
	}
	
	~ConcatNode() {}
	
	virtual REInst * compile();
};

#endif /* defined(__LexisFontinalis__ConcatNode__) */
