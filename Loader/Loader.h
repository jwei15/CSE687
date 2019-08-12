#pragma once
///////////////////////////////////////////////////////////////////////////
// Loader.h   : Find files that satisfies the regular expression         //
//              Recursively find the files' dependencies                 //
//              Store the filespecs in its vector                        //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 2                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Jinhao Wei                                            //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*   This package will allow us to find files that meets the regular expressions from command line
*   Then it will read into the files, and recursively find all dependencies
*   Then it will record all the files and their dependencies, for further uses
*
*  Required Files:
* =======================
*  DependencyTable.h  which helps us record the dependencies relationships
*  DirExplorerN.h  without this package, we will not be able to read from command line and find files
*  CodeUtilities.h  this package provides us with convenience to find file's fullname
*
*  Maintainence History:
* =======================
*  ver 1.0 - 20 Mar 2019
*  - first release
*/

#include <vector>
#include <iostream>

#include <string>
#include <regex>

//#include "../Project2Starter/Logger/Logger.h"
//#include "../Logger/Logger.h"
#include "../DependencyTable/DependencyTable.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

using namespace FileSystem;
using namespace CodeUtilities;
//using namespace Logging;

class Loader {
public:
	using Files = std::vector<std::string>;
	
	Loader() {};
	Loader(int argc, char** argv);
	Loader(std::vector<std::string> argv);
	~Loader();
	Files Filelist();

	void FindDependencies(std::string filename);
	void LoadDependencies();

	void ShowFiles();
	void ShowDependencies();

	bool HasCppDependencies(std::string tmp);
	
	std::vector<std::string> getFiles();
	void ReformFileVector();
	void AddFileRecord(std::string filename);

	std::string ReturnDirectory(std::string);

	DependencyTable getDT_();

private:
	DependencyTable* dt;
	ProcessCmdLine* pcl;	//Do not use "new" operation unless necessary 
	DirExplorerN* de;	//Do not use "new" operation unless necessary
	Files files_;	//files that match the regex
	Files dependencies;	//files that are dependencies of files in files_

};

