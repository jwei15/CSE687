#pragma once
///////////////////////////////////////////////////////////////////////////
// Folding.h   : Package that fold up our cpp codes                      //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 2                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Jinhao Wei                                            //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package provides a class named FoldingTool, which is responsible for folding up codes
*  When using this package, we will first initialize the FoldingTool instance, with a parse tree
*  Then we will use OpenFile(std::string Filename), AddDivSymbol(std::string Filename) 
*
*  Required Files:
* =======================
*  Parser.h  which gave us the information concerning where to add corresponding css/html code
*  AbstrSynTree.h  which gave us a clear version to traverse the parse tree
*
*  Maintainence History:
* =======================
*  ver 1.0 - 20 Mar 2019
*  - first release
*/



#include <iostream>
#include <fstream>
#include <string>
#include "../Project2Starter/Parser/Parser.h"
#include "../Project2Starter/AbstractSyntaxTree/AbstrSynTree.h"

using namespace CodeAnalysis;

class FoldingTool {
public:
	FoldingTool();
	FoldingTool(ASTNode* AbstrSynTreeRoot);
	~FoldingTool();

	int containsStartComment(std::string s);
	int containsEndComment(std::string s);

	void treewalk(ASTNode* root);
	bool AddDivSymbolToCode(std::string fileSpec);
	bool AddDivSymbolToComment(std::string fileSpec);
	bool OpenFile(std::string fileSpec);
	bool WriteFile(std::string filespec);
private:
	ASTNode* ParseTreeRoot;
	std::vector<std::string> FileBuffer;
};