// Folding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Folding.h"
#include "fstream"
#include <iostream>
#include <stack>

FoldingTool::FoldingTool() {

};

FoldingTool::FoldingTool(ASTNode* node) {
	ParseTreeRoot = node;
}

FoldingTool::~FoldingTool() {

}

void FoldingTool::treewalk(ASTNode* root) {
	std::ostringstream out;
	/* //I intent to use the stacks to handle some boundary conditions
		//such as void Print(){ //{   or   void Print(char s = '{') {
		//But that was not very easy
	std::stack<int> BraceStack;	
	std::stack<int> QuotationStack;
	*/	
	FileBuffer[root->endLineCount_ - 1].append("</div>");
	std::string s = FileBuffer[root->startLineCount_ - 1];

	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == '{') {
			std::string blockId = std::to_string(root->startLineCount_);
			FileBuffer[root->startLineCount_ - 1].insert(i,
				"<input id = _" + blockId + " type = \"checkbox\">" +
				"<label class = \"drop\" for = \"_" + blockId + "\"></label>" +
				"<div>");
		}
	}
	std::cout << out.str();
	auto iter = root->children_.begin();
	while (iter != root->children_.end())
	{
		treewalk(*iter);
		++iter;
	}
}

bool FoldingTool::AddDivSymbolToCode(std::string fileSpec) {
	OpenFile(fileSpec);
	treewalk(this->ParseTreeRoot);
	AddDivSymbolToComment(fileSpec);
	WriteFile(fileSpec);
	return true;
}

int FoldingTool::containsStartComment(std::string s) {
	for (int i = 0; i < (int)(s.length()); ++i) {
		if (s[i] == ' ' || s[i] == '\t') continue;
		else if (s[i] == '/' && i + 1 < s.length()) {
			if (s[i + 1] == '*') {
				return i;
			}
		}
	}
	return -1;
}

int FoldingTool::containsEndComment(std::string s) {
	for (int i = 0; i < (int)(s.length()); ++i) {
		if (s[i] == ' ' || s[i] == '\t') continue;
		else if (s[i] == '*' && i + 1 < s.length()) {
			if (s[i + 1] == '/') {
				return i;
			}
		}
	}
	return -1;
}

bool FoldingTool::AddDivSymbolToComment(std::string fileSpec) {
	std::stack<int> CommentLineNumber;
	for (int i = 0; i < FileBuffer.size(); ++i) {
		int pos1 = containsStartComment(FileBuffer[i]);
		int pos2 = containsEndComment(FileBuffer[i]);
		
		if (FileBuffer[i].substr(0, 5) == "/////") {
			if (CommentLineNumber.empty()) CommentLineNumber.push(i);
			else if (FileBuffer[CommentLineNumber.top()].substr(0, 5) == "/////") {
				std::string BlockId = std::to_string(i);
				FileBuffer[CommentLineNumber.top()].insert(0,
					"<input id = _" + BlockId + " type = \"checkbox\">" +
					"<label class = \"drop\" for = \"_" + BlockId + "\"></label>" +
					"<div>");
				FileBuffer[i].append("</div>");
				CommentLineNumber.pop();
			}else {
				CommentLineNumber.push(i);
			}
				
		}
		if (pos1 != -1) {
		
			CommentLineNumber.push(i);
		}
		if (pos2 != -1) {
			std::string BlockId = std::to_string(i);
			FileBuffer[CommentLineNumber.top()].insert(0,
				"<input id = _" + BlockId + " type = \"checkbox\">" +
				"<label class = \"drop\" for = \"_" + BlockId + "\"></label>" +
				"<div>");
			FileBuffer[i].append("</div>");
			CommentLineNumber.pop();
		}
	}
	return true;
}

bool FoldingTool::WriteFile(std::string fileSpec) {
	std::cout << "Writing File " << fileSpec << std::endl;
	std::ofstream of(fileSpec);
	int i = 0;
	while (i < FileBuffer.size()) {
		//std::cout << FileBuffer[i] << std::endl;
		of << FileBuffer[i];
		of << "\n";
		++i;
	}
	return true;
}

bool FoldingTool::OpenFile(std::string fileSpec) {
	std::ifstream in(fileSpec);
	std::string s;
	try {
		while (std::getline(in,s)) {
			this->FileBuffer.push_back(s);
			s.clear();
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}


//test stub

#ifdef FOLDING_H
int main(int argc, char** argv)
{

	for (int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}
    
}
#endif
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
