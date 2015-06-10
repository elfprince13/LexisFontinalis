//
//  ParenNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__ParenNode__
#define __LexisFontinalis__ParenNode__

#include "OperationNode.h"

class ParenNode : public OperationNode {
public:
	bool capture;
	ParenNode(bool capture, RENode* constructAround) :
	OperationNode(Paren, Left, 1), capture(capture) {
		children.push_back(constructAround);
	}
	
	~ParenNode() {}
	
	virtual REInst * compile();
};

#endif /* defined(__LexisFontinalis__ParenNode__) */
