#pragma once
///////////////////////////////////////////////////////////////////////////
// CodePublisher.h : defines facade/executive for OOD Pr1 S19            //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodePublisher which uses different components of
*  CodeConverter, Display and DirectoryN to put together the entire 
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade
*
*  NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor DEMO_CP : runs in full demo mode for Pr1.
*   2) Preprocessor USE_CP  : runs normal functionality. 
*
*  Required Files:
* =======================
*  CodePublisher.h CodePublisher.cpp CodeUtilities.h
*  Converter.h Converter.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Logger.h Logger.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#include <string>
#include <vector>

#include "../Loader/Loader.h"

#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

#include "ICodePublisher.h"

class CodePublisher:public INativeCodePublisher
{
public:
  //CodePublisher() ;
  bool processCommandLineArgs(int argc, char ** argv);
  
  void workingDirectory(const std::string& dir);
  const std::string& workingDirectory() const;

  void outputDirectory(const std::string& dir);
  const std::string& outputDirectory() const; 

  bool extractFiles();

  void publish();
  void publish(const std::string& file);
  void publish(const std::vector<std::string>& files);

  CodeUtilities::DisplayMode displayMode() const;

private:
  CodeUtilities::ProcessCmdLine *pcl_;
  CodeConverter cconv_;
  Display display_;

  std::string dirIn_; 
  std::string dirOut_;

  std::vector<std::string> files_;
};

