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
		input >> next;
		switch (next){
				// Do we have a repetition operator?
			case '?':
			case '*':
			case '+':
			case '{':
				read.oper = acceptRepOperator(input, next, err);
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
			case '~':
			case '[':
				read.data = acceptRange(input, next, err);
				break;
			case '.':
				read.data = new MatchNode("",true);
				break;
				
				// Do we have a literal?
			case '\'':
				read.data = acceptLiteral<LiteralNode, '\''>(input, err);
				break;
				
				
				// Do we have an identifier?
			default:
				false;
		}
		
		
		
		
		// Nothing should go after this
		dataLast = data;
		data = true;
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

template<char terminal> std::function<bool(std::istream&)> makeEscapeAccepter(bool &inEscape, int &ctSinceSlash){
	return [&](std::istream& i){
		bool ret;
		if(i.eof()){
			ret = false;
		} else {
			char next = i.peek();
			if(inEscape){
				if(ctSinceSlash){
					if(ctSinceSlash > 2){
						ret = false;
					} else {
						ret = isxdigit(next);
						ctSinceSlash = (ctSinceSlash + 1) % 2;
						inEscape = !ret || ctSinceSlash;
					}
				} else {
					switch(next){
						case 'r':
						case 'n':
						case 't':
						case 'v':
						case '\\':
						case terminal:
							inEscape = false;
							ret = true;
						case 'x':
							ctSinceSlash++;
							ret = true;
						default:
							ret = false;
					}
				}
			} else if(next == '\\') {
				inEscape = true;
				ret = true;
			} else {
				ret = (next != terminal) && (next >= ' ');
			}
		}
		
		return ret;
	};
}

template<class NodeType, char term> NodeType* REParse::acceptLiteral(std::istream& input, bool & err){
	NodeType * ret = nullptr;
	err = false;
	std::string read;
	std::ostringstream errStream;
	
	input >> std::noskipws;
	
	bool inEscape = false;
	int ctSinceSlash = 0;
	
	read = takeWhile(makeEscapeAccepter<term>(inEscape, ctSinceSlash), input);
	
	if(input.eof()){
		errStream << "Reached EOF before " << term << " while parsing a literal in " << __func__;
	} else if (input.peek() < ' '){
		errStream << "Reached a non-printable character before " << term << " while parsing a literal in " << __func__;
	} else if (inEscape){
		errStream << "Encountered an unknown control sequence " << term << " while parsing a literal in " << __func__;
	} else if (input.peek() != '\'') {
		errStream << "Unknown error while parsing a literal in " << __func__;
	}else {
		input.ignore();
	}
	
	errMsg = errStream.str();
	if(errMsg != ""){
		err = true;
	} else {
		ret = new NodeType(read);
	}
	
	input >> std::skipws;
	
	return ret;
}

MatchNode * REParse::acceptRange(std::istream &input, char instigator, bool &err) {
	MatchNode * ret = nullptr;
	err = false;
	
	bool negate = false;
	switch(instigator){
		case '~':
			negate = true;
			if(!input.eof() && input.peek() == '['){
				input.ignore();
			} else {
				err = true;
				errMsg = "acceptRange error: ~ must be followed by [";
				break;
			}
		case '[':
			ret = acceptLiteral<MatchNode, ']'>(input, err);
			ret->negate = negate;
			break;
		default:
			err = true;
			errMsg = "Unknown instigating character in acceptRange";
	}
	
	return ret;
}

RepeaterNode* REParse::acceptRepOperator(std::istream& input, char instigator, bool & err){
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

std::string REParse::takeWhile(std::function<bool(std::istream&)> predicate, std::istream& input){
	std::ostringstream readHere;
	while(predicate(input)){
		readHere.put(input.get());
	}
	return readHere.str();
}