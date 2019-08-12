//	IPublisher.cpp: Implementation of IPublisher.h methods
//	All methods in this file will essentially 
	//invoke already defined methods in native C++ projects

////////////////////////////////////////////////////////////////////////////////////////////
//	Our c++ project functions will be exposed in this way:                                //
//                                                                                        //
//	C++ Projects --> IPublisher class --> Interface class -->  Shim Class --> C# GUI	  //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

//	Created by: Jinhao Wei
//  Phone: 315-708-2851
//	Email: jwei15@syr.edu



#define IN_DLL
#include "IPublisher.h"
#include <iostream>

IPublisher::IPublisher() {
	ldr = new Loader();
	//cc = *(new CodeConverter());
};

// --------------converte a single file ---------------------
// --------------this function was never called in my project ----------------
bool IPublisher::convertFile(std::string filename) {
	cc = new CodeConverter();
	return cc->convertFile(filename);
}

// -------------- converte all the files that are recoreded in a CodeConverter --------------
int IPublisher::convert() {
	return cc->convert().size();
}

// --------------Intent to use this and pass command line to CodePublisher -------------------
// --------------Failed because the memory that char** argv pointed to won't last long --------------------------
// --------------I merely used this to test my dll and lib are correctly implemented -----------
// --------------Never use this --------------------------------------
std::vector<std::string> IPublisher::generateToBeLoaded(int argc, char** argv) {
	/*
	printf("\nIn IPublisher: %s \n", *(argv+1));
	*/
	char* x[] = { 
		(char*)"GUI", (char*)"E:\\687\\Project3_My\\Converter", (char*)"Converter.h" };

	ldr = new Loader(argc, x);
	cc = new CodeConverter(ldr->getDT_());
	return ldr->Filelist();
}

// ----------------- To return a vector of file name that should be converted ----------------
std::vector<std::string> IPublisher::generateToBeLoaded(std::vector<std::string> argv) {
	//char* x[] = {
	//	(char*)"GUI", (char*)"E:\\687\\Project3_My\\Converter", (char*)"Converter.h" };

	ldr = new Loader(argv);
	cc = new CodeConverter(ldr->getDT_());
	return ldr->Filelist();
}

// ------------------- To fold up all the converted files -------------------------------
void IPublisher::refineConvertedFiles() {
	using namespace CodeAnalysis;

	std::string ConvertedFilePath = "..\\..\\..\\..\\ConvertedWebpages\\";
	std::vector<std::string> FilesAndDependencies = Directory::getFiles(ConvertedFilePath, "*");
		//Directory::getFiles("../ConvertedWebpages/", "*");

	std::string fileSpec;
	std::vector<std::string> files;
	for (int i = 0; i < FilesAndDependencies.size(); ++i)
	{
		//if (FilesAndDependencies[i].find("html") != -1) continue;
		fileSpec = FileSystem::Path::getFullFileSpec(ConvertedFilePath + FilesAndDependencies[i]);
		std::string msg = "Processing file" + fileSpec;
		//std::cout << "\nIn IPublisher refiner: " << msg<<std::endl;
		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();	//In this step we construct a Parser and a Repository
		std::string name;
		try
		{
			if (pParser)
			{
				name = FileSystem::Path::getName(FilesAndDependencies[i]);
				if (!configure.Attach(fileSpec))
				{
					std::cout << "\n  could not open file " << name << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return;
			}
			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;
			while (pParser->next())
			{
				pParser->parse();
			}
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			FoldingTool ft(pGlobalScope);
			ft.AddDivSymbolToCode(fileSpec);

		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		files.push_back(fileSpec);
	}
	std::cout << "All files converted" << std::endl;
}

/*
bool IPublisher::SendFileThroughComm(std::string filename) {
	Comm comm(EndPoint("localhost", 9892), "client2Comm");
	comm.start();
	EndPoint serverEP("localhost", 9890);
	EndPoint clientEP("localhost", 9892);

	Message fileMsg(serverEP, clientEP);
	fileMsg.value("sendingFile");
	fileMsg.name("fileSender");
	fileMsg.file("comm.cpp");
	fileMsg.attribute("verbose", "blah");
	std::cout << "\n  " + comm.name() + " posting: " + fileMsg.file();
	comm.postMessage(fileMsg);

	size_t IMax = 3;
	for (size_t i = 0; i < IMax; ++i)
	{
		Message msg(serverEP, clientEP);
		msg.name("client #2 : msg #" + Utilities::Converter<size_t>::toString(i));
		std::cout << "\n  " + comm.name() + " posting:  " + msg.name();
		comm.postMessage(msg);
		Message rply = comm.getMessage();
		std::cout << "\n  " + comm.name() + " received: " + rply.name();
	}
	::Sleep(200);
}*/

DLL_DECL Interface* ObjectFactory::createInstance() {
	return new IPublisher; // ALLOWED BECAUSE NOW, ConcereteClass IS AN Interface TYPE
}
