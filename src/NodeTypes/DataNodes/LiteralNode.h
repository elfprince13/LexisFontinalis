//
//  LiteralNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__LiteralNode__
#define __LexisFontinalis__LiteralNode__

#include "DataNode.h"
#include <string>

class LiteralNode : public DataNode {
public:
	
	std::string match;
	LiteralNode(std::string m) : match(m) {
	}
	
	virtual ~LiteralNode() {
	}
	
	virtual REInst * compile();
};


#endif /* defined(__LexisFontinalis__LiteralNode__) */
