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
	REParse(std::map<std::string, RENode*> &eR);
	REParse();
	
	RENode * parseStream(std::istream& input);
	
private:
	struct Context {
		std::vector<RENode*> output;
		std::vector<OperationNode*> waiting;
	};
	
	std::map<std::string, RENode*> expandReferences;
	std::string errMsg;
	void contextualizeOperator(OperationNode* oper, Context& context, bool & err);
	
	RepeaterNode* acceptRepOperator(std::istream& input, char instigator, bool & err);
	
	MatchNode* acceptRange(std::istream& input, char instigator, bool & err);
	template<class NodeType, char term> NodeType* acceptLiteral(std::istream& input, bool & err);
	
	std::string takeWhile(std::function<bool(std::istream&)> predicate, std::istream& input);
	
};

#endif /* defined(__LexisFontinalis__REParse__) */
