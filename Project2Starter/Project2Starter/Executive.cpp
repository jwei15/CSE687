/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Starter code for Project #2                    //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

# include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "Executive.h"

using namespace Lexer;
using namespace Utilities;
//using Demo = Logging::StaticLogger<1>;

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper


using namespace CodeAnalysis;

#ifdef EXE_H
int main(int argc, char* argv[])
{
  Util::Title("Testing Project2Starter");
  putline();

  //Demo::attach(&std::cout);
  //Demo::start();

  // Analyzing files, named on the command line

  if (argc < 2)
  {
    std::cout
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  std::string fileSpec;

  for (int i = 1; i < argc; ++i)  // iterate over files
  {
    fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);

    ConfigParseForCodeAnal configure;
    Parser* pParser = configure.Build();	//In this step we construct a Parser and a Repository

    std::string name;

    try
    {
      if (pParser)
      {
        name = FileSystem::Path::getName(argv[i]);
        if (!configure.Attach(fileSpec))
        {
          std::cout << "\n  could not open file " << name << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }

      // save current package name
	  // In each iteration, there will be a new Repository
      Repository* pRepo = Repository::getInstance();
      pRepo->package() = name;

      // parse the package
      while (pParser->next())
      {
        pParser->parse();
      }


	  //FoldingTool* ft = new FoldingTool(pRepo->getGlobalScope());
	  //ft->AddDivSymbolToAllFiles();

      // final AST operations
      ASTNode* pGlobalScope = pRepo->getGlobalScope();

      // walk AST, computing complexity for each node
      // and record in AST node's element

      complexityEval(pGlobalScope);

      // Walk AST, displaying each element, indented
      // by generation

      TreeWalk(pGlobalScope);
    }
    catch (std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
      std::cout << "\n  exception caught at line " << __LINE__ << " ";
      std::cout << "\n  in package \"" << name << "\"";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}
#endif
