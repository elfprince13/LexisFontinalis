//
//  DataNode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__DataNode__
#define __LexisFontinalis__DataNode__

#include "RENode.h"
#include <string>


template<char term> std::string unescapeString(std::string& input){
	return input;
}

class DataNode : public RENode {
public:
	
	DataNode() {
	}
	
	virtual ~DataNode() {
	}
	
};


#endif /* defined(__LexisFontinalis__DataNode__) */
