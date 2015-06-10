//
//  REParse.cpp
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#include "REParse.h"
#include <sstream>

#include <locale>

REParse::REParse(std::map<std::string, RENode> &eR) : expandReferences(eR) {
}

RENode * REParse::parseStream(std::istream& input){
	Context context;
	context.output.clear();
	context.waiting.clear();
	
	RENode * ret = nullptr;
	bool err = false;
	bool data = true;
	bool dataLast = false;
	union {
		DataNode * data;
		OperationNode * oper;
	} read;
	char next;
	
	input >> std::skipws;
	while(!input.eof() && !err){
		dataLast = data;
		data = true;
		input >> next;
		switch (next){
				// Do we have a repetition operator?
			case '?':
			case '*':
			case '+':
			case '{':
				read.oper = acceptRepOperator(input, next, context, err);
				data = false;
				break;
				
				// Do we have a group?
			case '(':
				read.oper = new ParenNode(false, nullptr);
				data = false;
				break;
			
			case ')':
				read.oper = nullptr;
				break;
				
			// Do we have an alternation?
			case '|':
				read.oper = new AlternNode(nullptr, nullptr);
				data = false;
				break;
				
				// Do we have a wildcard?
			case '[':
			case '~':
			case '.':
				break;
				
				// Do we have a literal?
			case '\'':
				break;
				
				
				// Do we have an identifier?
			default:
				false;
		}
	}
	
	while(!context.waiting.empty() && !err){
		
	}
		
	if(err){
		std::cerr << errMsg << std::endl;
		if(ret != nullptr) {
			std::cerr << "Additionally, the erroring parse handler forgot to clean up" << std::endl;
			delete ret;
		}
		ret = nullptr;
	}
	
	return ret;
}

RepeaterNode* REParse::acceptRepOperator(std::istream& input, char instigator, Context & context, bool & err){
	RepeaterNode * ret = nullptr;
	err = false;
	
	bool greedy = true;
	char min = 1;
	char max = -1;
	
	std::ostringstream errStream;
	
	switch(instigator){
		case '?':
			max = 1;
		case '*':
			min = 0;
		case '+':
			greedy = (input.eof() || (input.peek() != '?')); // No space allowed here
			if(!greedy) input.ignore();
			break;
			
		case '{': {
			char firstArg, secondArg;
			input >> firstArg; /*= takeWhile([](std::istream& i){
								return !i.eof() && isdigit(i.peek());
								}, input);*/
			if(input.good()){
				input >> std::ws;
				if(input.eof()){
					errStream << "Reached EOF parsing: " << instigator << " in " << __func__;
				} else{
					switch(input.peek()){
						case ',': {
							input >> secondArg;
							if(input.good()){
								input >> std::ws;
								if(input.eof()){
									errStream << "Reached EOF parsing: " << instigator << " in " << __func__;
								} else{
									switch(input.peek()){
										case '}':
											input.ignore();
											break;
										default:
											errStream << "Invalid succeeding character to second argument " << input.peek() << " in " << __func__;
									}
								}
								
							}
						} break;
						case '}':
							input.ignore();
							secondArg = firstArg;
							break;
						default:
							errStream << "Invalid succeeding character to first argument " << input.peek() << " in " << __func__;
					}
				}
				
			}
			
			min = firstArg;
			max = secondArg;
			
			break;
		}
		default:
			// Error condition! We shouldn't be here
			errStream << "Unknown instigating character: " << instigator << " in " << __func__;
	}
	
	errMsg = errStream.str();
	if(errMsg != ""){
		err = true;
	} else {
		ret = new RepeaterNode(min, max, greedy, nullptr);
	}
	
	
	return ret;
}

std::string REParse::takeWhile(bool (*predicate)(std::istream&), std::istream& input){
	std::ostringstream readHere;
	while(predicate(input)){
		readHere.put(input.get());
	}
	return readHere.str();
}