#pragma once

//	Interface.h provides a class named Interface
//	Interface class serves as the connection 
//	All methods within this class will be implemented in IPublisher class
//	



#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <iostream>

class Interface {
public:
	//virtual void generateLoader() = 0; // pure virtual, no implementation
	virtual bool convertFile(std::string filename) = 0;
	virtual int convert() = 0;
	virtual void refineConvertedFiles() = 0;
	virtual std::vector<std::string> generateToBeLoaded(int argc, char** argv) = 0;
	virtual std::vector<std::string> generateToBeLoaded(std::vector<std::string> argv) = 0;
	virtual ~Interface() {};

};


extern "C" {
	struct ObjectFactory {
		DLL_DECL Interface* createInstance(); // NOTE THAT WE HAVE NO KNOWLEDGE OF ConcreteClass HERE
	};
};