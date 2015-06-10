//
//  IdentNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__IdentNode__
#define __LexisFontinalis__IdentNode__

#include "DataNode.h"
#include <string>
#include <map>

class IdentNode : public DataNode {
public:
	
	std::string ident;
	std::map<std::string, RENode*> &lookup;
	
	bool capture;
	
	IdentNode(std::string i, std::map<std::string, RENode*> &env, bool c = false) : ident(i), lookup(env), capture(c) {
	}
	
	virtual ~IdentNode() {
	}
	
	virtual REInst * compile();
};


#endif /* defined(__LexisFontinalis__IdentNode__) */
