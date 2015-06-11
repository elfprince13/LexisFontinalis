//
//  OperationNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__OperationNode__
#define __LexisFontinalis__OperationNode__

#include "RENode.h"
#include <iostream>


class OperationNode : public RENode {
	
private:
	int64_t childCount;
public:
	
	enum Precedence {
		Least = 0,
		Alternate = 1,
		Concat = 2,
		Repeat = 3,
		Paren = 4
	};
	
	enum Associativity {
		None = 0,
		Left = 1,
		Right = 2
	};
	
	std::vector<RENode*> children;
	Precedence precedence;
	Associativity associativity;
	
	OperationNode(Precedence p = Least, Associativity a = None, int64_t c = 0) : precedence(p), associativity(a), childCount(c) {
		children.clear();
	}
	
	int64_t numChildren() {
		if (childCount != children.size()/* && childCount != -1*/){
			std::cerr << "Childcount mismatch" << std::endl;
		}
		return childCount;
	};
	
	virtual ~OperationNode() {
		std::for_each(children.begin(), children.end(), [](RENode* childHere){ if(childHere != nullptr){ delete childHere; } });
		children.clear();
	}
};

#endif /* defined(__LexisFontinalis__OperationNode__) */
