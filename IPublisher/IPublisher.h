#pragma once
//	IPublisher.h provides a class named IPublisher
//	IPublisher class inheritates from Interface class
//	All virtual functions in Interface class will be implemented in IPublisher class

//	IPublisher class will be referenced by Shim Class in Shim.h,
	// so our c++ project functions can be invoked in Shim, then Shim will translate them to C# 
		//for GUI uses

//  This class mainly provides loading and converting methods for Shim class

//	Author: Jinhao Wei
//	E-mail: jwei15@syr.edu

#include <vector>
#include <iostream>
#include "../Interface/Interface.h"

#include "../Converter/Converter.h"
#include "../Loader/Loader.h"

#include "../Project2Starter/AbstractSyntaxTree/AbstrSynTree.h"
#include "../Project2Starter/Parser/ActionsAndRules.h"
#include "../Project2Starter/Parser/ConfigureParser.h"
#include "../Project2Starter/Parser/Parser.h"
#include "../Folding/Folding.h"


//#include "../Comm/MsgPassingComm/Comm.h"
//#include "../Comm/MsgPassingComm/Comm.h"

class IPublisher:public Interface {
public:
	IPublisher();
	bool convertFile(std::string filename);
	int convert();
	std::vector<std::string> generateToBeLoaded(int argc, char** argv);
	std::vector<std::string> generateToBeLoaded(std::vector<std::string> argv);
	void refineConvertedFiles();

private:
	Loader* ldr;
	CodeConverter* cc;
};