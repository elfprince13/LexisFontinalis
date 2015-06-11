//
//  RENode.h
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#ifndef __LexisFontinalis__RENode__
#define __LexisFontinalis__RENode__

#include <iostream>
#include <vector>
#include <functional>

#include "../REISA/REInst.h"

class RENode {
public:
	
	virtual REInst * compile() = 0;
	
	virtual ~RENode() {}
	
	template<class NodeType> static void purgeNodeVector(std::vector<NodeType*> & vec, bool warnNull=false){
		std::for_each(vec.begin(), vec.end(),
					  [=](NodeType* elm){
						  if(elm != nullptr){
							  delete elm;
						  } else if(warnNull){
							  std::cerr << "Skipping delete of nullptr in " << __func__ << std::endl;
						  }
					  });
		vec.clear();
		
	}
};

#endif /* defined(__LexisFontinalis__RENode__) */
