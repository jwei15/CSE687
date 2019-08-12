#include "Executive.h"
#include <iostream>

#define Util StringHelper



void LoaderDemo() {

}


int main(int argc, char* argv[])
{
	Util::Title("We are in package Executive");
	putline();

	Loader ldr(argc, argv);
	ldr.ShowDependencies();

	CodeConverter cc(ldr.getDT_());
	cc.convert();
	//This step returns only the name of the file
	std::vector<std::string> FilesAndDependencies = 
		Directory::getFiles("../ConvertedWebpages/", "*");

	std::string fileSpec;
	std::vector<std::string> files;
	for (int i = 0; i < FilesAndDependencies.size(); ++i)
	{
		fileSpec = FileSystem::Path::getFullFileSpec("../ConvertedWebpages/" +FilesAndDependencies[i]);
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
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
				return 1;
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
	Display d = Display();
	d.display(files);
}

