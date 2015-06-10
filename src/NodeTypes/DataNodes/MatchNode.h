//
//  MatchNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__MatchNode__
#define __LexisFontinalis__MatchNode__

#include "DataNode.h"
#include <string>

class MatchNode : public DataNode {
public:
	bool negate;
	std::string match;
	MatchNode(std::string match, bool negate = false) : match(match), negate(negate) {
	}
	
	virtual ~MatchNode() {
	}
	
	virtual REInst * compile();
};

#endif /* defined(__LexisFontinalis__MatchNode__) */
