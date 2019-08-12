

#include <iostream>
#include <fstream>
#include "Loader.h"

Loader::~Loader() {

}

Loader::Files Loader::Filelist() {
	return files_;
}

Loader::Loader(std::vector<std::string> argv) {

	pcl = new ProcessCmdLine(argv);
	std::cout << "In Loadercpp: " << pcl->path() << std::endl;
	de = new DirExplorerN(pcl->path());
	dt = new DependencyTable();

	for (auto patt : pcl->patterns())
	{
		de->addPattern(patt);
	}

	if (pcl->hasOption("s"))
	{
		de->recurse();
	}

	bool res = de->search() && de->match_regexes(pcl->regexes());

	auto& tmp = de->filesList();
	for (auto it = tmp.begin(); it != tmp.end(); ++it) {
		files_.push_back(*it);
	}

	LoadDependencies();
	ReformFileVector();
	//std::cout << "In Loadercpp: " << files_.size() << std::endl;
}


Loader::Loader(int argc, char** argv) {

	pcl = new ProcessCmdLine(argc, argv);
	std::cout << "In Loadercpp: " << pcl->path() << std::endl;
	de = new DirExplorerN(pcl ->path());
	dt = new DependencyTable();

	for (auto patt : pcl->patterns())
	{
		de->addPattern(patt);
	}

	if (pcl->hasOption("s"))
	{
		de->recurse();
	}

	bool res = de->search() && de->match_regexes(pcl->regexes());

	auto& tmp = de->filesList();
	for (auto it = tmp.begin(); it != tmp.end(); ++it) {
		files_.push_back(*it);
	}
		
	LoadDependencies();
	ReformFileVector();
	std::cout << "In Loadercpp: " << files_.size() << std::endl;
}

//To check if each line in a .cpp or .h file contains #include statement
bool Loader::HasCppDependencies(std::string str) {
	for (int i = 0; i < (int)(str.length()); ++i) {
		if (str[i] == ' ' || str[i] == '\t') continue;
		else if (str[i] == '#') {
			break;
		} else{
			//std::cout << str[i] << std::endl;
			return false; 
		}
	}
	//if (str[0] != '#') return false;
	//# include "", we should pay attention to spaces or tabs between # and include
	return (std::regex_match(str, std::regex("( *|\t*)#( *|\t*)include( *|\t*)\".*\"")));
}

void Loader::ShowDependencies() {
	dt->display();
}

//Return the relative directory where this file is in
std::string Loader::ReturnDirectory(std::string filename) {
	return filename.substr(0, filename.rfind("\\") + 1);
}

//Given a cpp filename, recursively find dependencies for it
//"recursively" means this will find its dependencies, its dependencies' dependencies ...
void Loader::FindDependencies(std::string filepath) {
	std::ifstream in(filepath);
	std::string relative_path = ReturnDirectory(filepath);
	std::string tmp;

	std::cout << "Finding Dependencies for file: " << filepath << std::endl;
	if (in) {
		while (std::getline(in, tmp)) {
			if (HasCppDependencies(tmp)) {
				//Get dependency's file name, just a file name, not a path
				std::string dependency = tmp.substr(tmp.find("\"") + 1,
					tmp.rfind("\"") - tmp.find("\"") - 1);

				//Append dependency's file name to relative directory, and return absolute path
				std::string dependency_abs_path = Path::getFullFileSpec(relative_path + dependency);
				//AddFileRecord(dependency_abs_path);
				dt->addDependency(filepath, dependency_abs_path);
				//Recursively find dependencies
				FindDependencies(dependency_abs_path);
			}
		}
	}
	in.close();
}

//Add a file record to files_
void Loader::AddFileRecord(std::string filename) {
	if (std::find(files_.begin(), files_.end(), filename) != files_.end()) {
		return;
	}
	files_.push_back(filename);
}

/*-- For each files in this->files_, recursively add its dependencies --*/
void Loader::LoadDependencies() {
	if (files_.size() == 0) return;
	for (auto file_ : files_) {
		FindDependencies(file_);
	}
}

DependencyTable Loader::getDT_(){
	return *dt;
}

void Loader::ReformFileVector() {
	files_.clear();
	for (auto entry : *dt) {
		AddFileRecord(entry.first);
		for (auto dep : entry.second) {
			AddFileRecord(dep);
		}
		//std::cout << std::endl;
	}
}

std::vector<std::string> Loader::getFiles() {
	return files_;
}

void Loader::ShowFiles() {
	//Demo::write("Printing Loaded files\n");
	int length = files_.size();
	if (length == 0) {
		std::cout << "No files loaded\n";
		return;
	}
	for (int i = 0; i < length; ++i) {
		std::cout << files_[i] << "\n";
	}
}

/*---------test stub--------------
#ifdef LOADER_H
int main(int argc, char** argv)
{
	//LoggerDebug::attach(&std::cout);
	//LoggerDebug::start();

	LoggerDemo::attach(&std::cout);
	LoggerDemo::start();

	Loader ldr(argc, argv);
	//ldr.FindDependencies("../Converter/Converter.h");
	ldr.ShowFiles();
	ldr.ShowDependencies();
	return 0;
}
#endif
*/

