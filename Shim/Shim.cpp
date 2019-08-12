//	Shim.cpp: Implementation of Shim class methods
//  All methods within this class will translate c++ code to c# version
//	All methods within this class will be directively available to c# WPF

//  Our logic here is that:
//	WPF calls Shim class methods, then Shim class methods returns a translated c++ project result



#define IN_DLL
#include "Shim.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

Shim::Shim() {
	ObjectFactory factory;
	pointer_ = factory.createInstance();
}

Shim::~Shim() {
	if (pointer_ != nullptr)
		delete pointer_;
	pointer_ = nullptr;
}

void Shim::test() {
	std::cout << "test" << std::endl;
}



std::string Shim::mToN(String^ str)
{
	//return msclr::interop::marshal_as<std::string>(str);
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
	
}

String^ Shim::nToM(const std::string& str)
{
	StringBuilder^ sb = gcnew StringBuilder("");
	for (size_t i = 0; i < str.size(); ++i)
		sb->Append((wchar_t)str[i]);
	return sb->ToString();
}

//To convert String^(CLI) List(CLI) to std string(C++) vector(C++)
std::vector<std::string>Shim::marrayToNarray(List<String^>^ str) {
	std::vector<std::string> result;
	for each (String^ var in str)
	{
		result.push_back(mToN(var));
	}
	return result;
}


//To convert std string vector(C++) to Stirng^ List (CLI)
List<String^>^ Shim::narrayToMarray(const std::vector<std::string>& str) {
	List<String^>^ test = gcnew List<String^>();
	for (int i = 0; i < str.size(); ++i) {
		test->Add(nToM(str[i]));
	}
	return test;
}

// --------- Intent to use this to pass char** argv to Loader for dependency analysis ---------
// --------- Never use this because the corresponding memory  will be released before being passed on---------
char** Shim::std_string_vec_To_p_p_char(const std::vector<std::string>& str) {
	std::vector<char*> cstrings;
	cstrings.reserve(str.size());
	//char* test[10];
	for (size_t i = 0; i < str.size(); ++i) {
		cstrings.push_back(const_cast<char*>(str[i].c_str()));
		//test[i] = const_cast<char*>(str[i].c_str());
	}
	
	printf("\nIn Shim: %s at position %p\n", &cstrings[1][0], &cstrings[1]);
	return &cstrings[0];
}

//<-------- This is to return all the files to be converted, including cpp files and its dependencies-----------------> //
List<String^>^ Shim::ToBeConverted(List<String^>^ argv) {
	int i = 0;
	for each (String^ var in argv)
	{
		i = i + 1;
		Console::Write(var);
	}
	return narrayToMarray(pointer_->generateToBeLoaded(marrayToNarray(argv)));
//	return narrayToMarray(pointer_->generateToBeLoaded(i,
		//std_string_vec_To_p_p_char(marrayToNarray(argv))));
}

// < -----------Convert all the files to html, without dependencies ----------------->
int Shim::convert() {
	int count = pointer_->convert();
	flag = count;
	return count;
}

//  <-----------Add dependencies to each of the html files in ConvertedWebPages -------->
void Shim::refineConvertedFiles() {
	if (flag > 0)
		pointer_->refineConvertedFiles();
}

Shim^ ShimFactory::createShim()
{
	return gcnew Shim();
}



//#ifdef SHIMH
int main() {
	Shim^ shim = ShimFactory::createShim();
	List<String^>^ test = gcnew List<String^>();
	//std::vector<std::string> tobeConverted = shim->generateToBeLoaded(test);

	//Console::Write(shim->convertFile(tobeConverted[0]));
	return 0;
}
//#endif