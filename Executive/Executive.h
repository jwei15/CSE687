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
*  There is a main functin here, which is the entrance of this entire project
*
*  Required Files:
* =======================
*  Loader.h  load files(from command line) and their dependencies
*  CodePublisher.h  
*  Converter.h  convert files from .h or .cpp to .html
*  Folding.h    fold up codes in .html format
*  Display.h    show files content
*  FileSystem.h    a overall package helps us to maintain the directories and files
*  Parser.h     parse the files, and record informations on class definitions, etc
*  AbstrSynTree.h   a data structure that helps us maintain the informations parsed
*  Utilities.h   
*  ActionsAndRules.h  defines the rules that a parse should follow, and solve the problems with actions
*  ConfigureParser.h  build up a parser
*  CodeUtilities.h

*  Maintainence History:
* =======================
*  ver 1.0 - 25 Mar 2019
*  - first release
*/
#include "../Loader/Loader.h"
#include "../CodePublisher/CodePublisher.h"
#include "../Converter/Converter.h"
#include "../Folding/Folding.h"
#include "../Display/Display.h"
#include "../FileSystem/FileSystem.h"

#include "../Project2Starter/Parser/Parser.h"
#include "../Project2Starter/AbstractSyntaxTree/AbstrSynTree.h"

#include "../Project2Starter/Utilities/Utilities.h"
#include "../Project2Starter/Parser/ActionsAndRules.h"
#include "../Project2Starter/Parser/ConfigureParser.h"
#include "../Project2Starter/Logger/Logger.h"

#include "../Utilities/CodeUtilities/CodeUtilities.h"

using namespace FileSystem;
using namespace CodeAnalysis;
using namespace Logging;