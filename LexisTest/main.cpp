//
//  main.cpp
//  LexisTest
//
//  Created by Thomas Dickerson on 6/10/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#include <iostream>
#include <sstream>


#include "../src/NodeTypes/NodeDefs.h"
#include "../src/REParse.h"


void paindemo(){
	std::string s("1");
	
	{
		std::istringstream exp(s);
		exp.ignore();
		std::cout << "1)" << "\t" << exp.eof() << "\t" << (exp.peek() == EOF) << std::endl;
	}
	
	{
		std::istringstream exp(s);
		exp.ignore();
		std::cout << "2)" << "\t" << (exp.peek() == EOF)  << "\t" << exp.eof() << std::endl;
	}
	
	{
		std::istringstream exp(s);
		exp.ignore();
		char c = '0';
		std::cout << "3)" << "\t" << (c = exp.peek(), c);
		std::cout << "\t" << (exp >> c, c) << std::endl;
	}
	
	{
		std::istringstream exp(s);
		char c = '0';
		std::cout << "4)" << "\t" << (exp >> c, c);
		std::cout << "\t" << (exp >> c, c) << std::endl;
	}
	
	{
		std::istringstream exp(s);
		exp.ignore();
		char c = '0';
		std::cout << "5)" << "\t" << (exp >> c, c);
		std::cout << "\t" << (c = exp.peek(), c) << std::endl;
	}
	
}

int main(int argc, const char * argv[]) {
		//paindemo(); exit(0);
	
	std::istringstream exp1("'5'");
	std::istringstream exp2("('5' five)+? ~[123]");
	
	std::map<std::string, RENode*> dict1, dict2;
	dict1.clear();
	dict2.clear();
	REParse rp(dict1);
	RENode * out = rp.parseStream(exp1);
	
	std::cout << "First parse done" << std::endl;
	if(out != nullptr){
		dict2["five"] = out;
		
		REParse rp2(dict2);
		out = rp2.parseStream(exp2);
		
		
		std::cout << "Second parse done" << std::endl;
		
	}
	
	
    return 0;
}
