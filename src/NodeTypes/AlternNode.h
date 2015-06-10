//
//  AlternNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__AlternNode__
#define __LexisFontinalis__AlternNode__

#include "OperationNode.h"

class AlternNode : public OperationNode {
public:
	AlternNode( RENode* first, RENode *second) :
	OperationNode(Alternate, Left, 2) {
		children.push_back(first);
		children.push_back(second);
	}
	
	~AlternNode() {}
	
	virtual REInst * compile();
};

#endif /* defined(__LexisFontinalis__AlternNode__) */
