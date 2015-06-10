//
//  REParse.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__REParse__
#define __LexisFontinalis__REParse__

#include "NodeTypes/NodeDefs.h"
#include <iostream>
#include <map>

class REParse {
public:
	REParse(std::map<std::string, RENode>& eR);
	
private:
	struct Context {
		std::vector<RENode*> output;
		std::vector<OperationNode*> waiting;
	};
	
	std::map<std::string, RENode> &expandReferences;
	std::string errMsg;
	RENode * parseStream(std::istream& input);
	
	RepeaterNode* acceptRepOperator(std::istream& input, char instigator, Context & context, bool & err);
	
	std::string takeWhile(bool (*predicate)(std::istream&), std::istream& input);
	
};

#endif /* defined(__LexisFontinalis__REParse__) */
