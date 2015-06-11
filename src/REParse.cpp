//
//  REParse.cpp
//  LexisFontinalis
//
//  Created by Thomas Dickerson on 6/9/15.
//  Copyright (c) 2015 StickFigure Graphic Productions. All rights reserved.
//

#include "REParse.h"
#include "StreamUtil.h"

#include <sstream>
#include <locale>

REParse::REParse() : expandReferences() {
}

REParse::REParse(std::map<std::string, RENode*> &eR) : expandReferences(eR) {
}

void REParse::contextualizeOperator(OperationNode* oper, Context& context, bool & err){
	size_t i;
	for(i = oper->numChildren(); i > 0 && !context.output.empty(); i--){
		RENode* child = context.output.back();
		context.output.pop_back();
		oper->children[i] = child;
	}
	if(context.output.empty() && i != 0){
		err = true;
		errMsg = "ran out of children to populate operator";
	} else {
		context.output.push_back(oper);
	}
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
				if(isalpha(next) || '_' == next){
					std::ostringstream identstream;
					identstream << next;
					identstream << takeWhile([](std::istream& input){
						char peek;
						return !input.eof() && (peek = input.peek(), isalnum(peek) || '_' == peek);
					}, input);
					std::string identName = identstream.str();
					if(expandReferences.count(identName)){
						read.data = new IdentNode(identName, expandReferences);
					} else {
						err = true;
						errMsg = "Unknown identifier " + identName;
					}
				} else {
					err = true;
					errMsg = "Identifiers must begin with an alphabetical character or _";
					read.data = nullptr;
				}
		}
		
		
		if(!err){
			// Stack manipulation here.
			OperationNode * o1 = (!data) ? read.oper : ((data && dataLast) ? (new ConcatNode(nullptr, nullptr)) : nullptr), *o2;
			if(o1 != nullptr){
				while(!context.waiting.empty() &&
					  (o2 = context.waiting.back(),
					   o2->precedence != OperationNode::Paren && // Never pop parens
					   (o1->precedence < o2->precedence || // Never pop anything else for parens either, because paren has highest precdence
						(o1->precedence == o2->precedence &&
						 o1->associativity < o2->associativity)))) {
							context.waiting.pop_back();
							contextualizeOperator(o2, context, err);
						}
				
				context.waiting.push_back(o1);
			} else if (!data){
				// Right Paren
				bool success = false;
				while(!context.waiting.empty() && !success && !err){
					OperationNode * oper = context.waiting.back();
					context.waiting.pop_back();
					if(oper->precedence == OperationNode::Paren){
						success = true;
					}

					contextualizeOperator(oper, context, err);
				}
				if(!success){
					errMsg = (err ? (errMsg + "\nAdditionally encountered, ") : "") + "Mismatched parens"; // No need for
					err = true;
				}
			}
			
			// Don't chain these together: we might have implicitly introduced a ConcatNode.
			
			if(data){
				context.output.push_back(read.data);
			}
			
		}
		
		
		// Nothing should go after this
		dataLast = data;
		data = true;
	}
	
	while(!context.waiting.empty() && !err){
		OperationNode * oper = context.waiting.back();
		context.waiting.pop_back();
		if(oper->precedence == OperationNode::Paren){
			err = true;
			errMsg = "Mismatched parens";
		} else {
			contextualizeOperator(oper, context, err);
		}
	}
	
	if(!err && context.output.size() != 1){
		err = true;
		errMsg = "Something went wrong: there should only be one output element";
	} else if(!err) {
		ret = context.output.back();
		context.output.pop_back();
	}
	
	if(err){
		std::cerr << errMsg << std::endl;
		if(ret != nullptr) {
			std::cerr << "Additionally, the erroring parse handler forgot to clean up" << std::endl;
			delete ret;
		}
		RENode::purgeNodeVector(context.output,true);
		RENode::purgeNodeVector(context.waiting,true);
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
	} else{
		char peek = input.peek();
		if (peek < ' '){
			errStream << "Reached a non-printable character before " << term << " while parsing a literal in " << __func__;
		} else if (inEscape){
			errStream << "Encountered an unknown control sequence " << term << " while parsing a literal in " << __func__;
		} else if (peek != term) {
			errStream << "Unknown error while parsing a literal in " << __func__;
		} else {
			SAFE_IGNORE(input);
			/*
			 bool eof1 = input.peek() == EOF;
			 bool eof2 = input.eof();
			 char peek2 = input.peek();
			char extrapeek2;
			input >> extrapeek2;
			bool eof3 = input.eof();
			bool eof4 = input.peek() == EOF;
			input.ignore();
			char peek3 = input.peek();
			bool eof5 = input.eof();
			bool eof6 = input.peek() == EOF;*/
			//std::cerr << peek << " " << eof1 << " " << eof2 << /*"\t" << peek2 << " " << extrapeek2 << " " << eof3 << " " << eof4 << "\t" << peek3 << " " << eof5 << " " << eof6 << */std::endl;
			
			//input.ignore();
		}
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
				SAFE_IGNORE(input);
			} else {
				err = true;
				errMsg = "acceptRange error: ~ must be followed by [";
				break;
			}
		case '[':
			ret = acceptLiteral<MatchNode, ']'>(input, err);
			if(!err) ret->negate = negate;
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
			if(!greedy) SAFE_IGNORE(input);
			break;
			
		case '{': {
			char firstArg, secondArg;
			input >> firstArg; 
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
											SAFE_IGNORE(input);
											break;
										default:
											errStream << "Invalid succeeding character to second argument " << input.peek() << " in " << __func__;
									}
								}
								
							}
						} break;
						case '}':
							SAFE_IGNORE(input);
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