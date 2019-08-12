#pragma once

//	Shim.h provides a class named Shim
//	Shim class will be directively referenced by our c# WPF
//	Shim class serves as a translator, receives a c# call 
	//and returns c++ function results in a c# way

//	Author: Jinhao Wei

//#include <string>
//#include <iostream>

#include "../Interface/Interface.h"


using namespace System;
using namespace System::Collections::Generic;

public ref class Shim {
public:
	Shim();
	~Shim();
	//bool convertFile(String^ filename);
	List<String^>^ ToBeConverted(List<String^>^ argv);

	int convert();
	void refineConvertedFiles();

	
	int flag = 0;
	void test();
private:	
	
	std::string mToN(String^ str);
	String^ nToM(const std::string& str);
	List<String^>^ narrayToMarray(const std::vector<std::string>& str);
	std::vector<std::string> marrayToNarray(List<String^>^ str);
	char** std_string_vec_To_p_p_char(const std::vector<std::string>& str);
	Interface* pointer_;
	//char* testchar[] = {};
};


public ref class ShimFactory
{
public:
	static Shim^ createShim();
};