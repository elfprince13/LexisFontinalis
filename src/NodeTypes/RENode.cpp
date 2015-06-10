//
//  RENode.cpp
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#include <iostream>
#include "NodeDefs.h"

/*

RENode * RENodeFactory::operator() (RENode * constructAround){
	RENode * out = factoryFunc(constructAround);
	if(out == nullptr) {
		// We should probably delete here to save replicated effort,
		// unless we want to allow some kind of post-mortem?
		std::cerr << "Factory func failed" << std::endl;
	}
	return out;
}

RENodeFactory RENodeFactory::operator() (RENodeFactory composeWith){
	return makeErrorFactory("woops");
	
}

RENodeFactory RENodeFactory::makeErrorFactory(std::string errContent){
	auto errFunc([=](RENode * constructAround){
		std::cerr << errContent << std::endl;
		return nullptr;
	});
	RENodeFactory ret(errFunc);
	return ret;
}

RENodeFactory RENodeFactory::makeRepeatFactory(char min, char max, bool greedy){
	auto repeatFunc([=](RENode * constructAround){
		RENode * repeatNode = new RepeaterNode(min, max, greedy, constructAround);
		return repeatNode;
	});
	
	RENodeFactory ret(repeatFunc);
	return ret;
}

*/