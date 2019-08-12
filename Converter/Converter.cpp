///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include "../Folding/Folding.h"
#include "../Project2Starter/Logger/Logger.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
using namespace Logging;
using namespace CodeAnalysis;


// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
	Dbug::write("\n  CodeConverter initialized with DependencyTable\n");
  //Dbug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
	//std::cout << "\nConvert called" << std::endl;
  if (!createOutpurDir()) {
    Dbug::write("\n\n  Failed to start conversion due to inability to create output directory");
    Dbug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
	std::cout<<"createOutpurDir not raised"<<std::endl;
    return std::vector<std::string>();
  }

  Demo::write("\n\n  Converting files to webpages");
  Dbug::write("\n\n  Converting files to webpages");

  Dbug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
      Demo::write("\n  -- Converted: " );
      Dbug::write("\n  -- Converted: " );
    }
    else {
      Demo::write("\n  -- Failed:    ");
      Dbug::write("\n  -- Failed:    ");
    }

    std::string filename = Path::getName(entry.first);
    Dbug::write(filename);
    Demo::write(filename);

	for (auto dependencies : entry.second) {
		if (convertFile(dependencies)) {
			Demo::write("\n    -- Dependency Converted: ");
		}
		else {
			Demo::write("\n    -- Dependency Failed: ");
		}
		Demo::write(dependencies);
	}

  }
  Dbug::write("\n");
  Demo::write("\n");
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (!createOutpurDir()) {
    Dbug::write("\n\n  Failed to start conversion due to inability to create output directory");
    Dbug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return "";
  }

  Demo::write("\n\n  Converting files to webpages");
  Dbug::write("\n\n  Converting files to webpages");

  Dbug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  if (convertFile(filepath)) {
    Demo::write("\n  -- Converted: ");
    Dbug::write("\n  -- Converted: ");
  }
  else {
    Demo::write("\n  -- Failed:    ");
    Dbug::write("\n  -- Failed:    ");
  }

  std::string filename = Path::getName(filepath);
  Dbug::write(filename);
  Demo::write(filename);

  return filename + ".html";
}



std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
    Dbug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  Dbug::write("\n  Output directory already exists. Proceeding...");
  return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
  return convertedFiles_;
}

void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}

// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{
	std::cout << "converting file " << file << std::endl;
  std::ifstream in(file);
  if (!in.is_open() || !in.good()) {
    Dbug::write("Error -- unable to read file, path may be invalid.");
    in.close();
    return false;
  }

  std::string filename = Path::getName(file);
  std::string outputPath = outputDir_ + filename + ".html";
  out_.open(outputPath, std::ofstream::out);
  if (!out_.is_open() || !out_.good()) {
    Dbug::write("Error -- unable to open output file for writing.");
    in.close();
    return false;
  }
  
  // write everything to file 
  addPreCodeHTML(filename);
  addDependencyLinks(file);
  addPreTag();

  while (in.good()) {
    std::string line;
    while (std::getline(in, line)) {
      skipSpecialChars(line);
      out_ << line << std::endl;
    }
  }

  addClosingTags();
  out_.close();

  convertedFiles_.push_back(outputPath);


  return true;
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<DOCTYPE !HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:15px 40px;" << std::endl;
  out_ << "        font-family: Consolas;" << std::endl;
  out_ << "        font-size: 1.25em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "      <link rel=\"stylesheet\"  href=\"../foldingformat.css\" />" << std::endl;
  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{
  std::string filename = Path::getName(file);
  if (!dt_.has(file)) { // in case of single file conversion
    Dbug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
    return;
  }

  if (dt_[file].size() == 0) { // in case the file has no dependencies
    Dbug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
    return;
  }

  out_ << "    <h3>Dependencies: " << std::endl;
  for (auto dep : dt_[file]) {
    out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
  }
  out_ << "    </h3>";
}

// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}


#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
  Dbug::attach(&std::cout);
  Dbug::start();

  Dbug::title("Testing CodeConverter functions");

  Dbug::stop(); // do not log DependencyTable logs
  
  DependencyTable dt;
  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  Dbug::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  Dbug::write("\n");
  //cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
  cc.convert();

  Dbug::stop();
  return 0;
}

#endif