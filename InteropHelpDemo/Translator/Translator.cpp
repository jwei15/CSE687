/////////////////////////////////////////////////////////////////
// Translator.cpp - Translates managed calls into native calls //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019   //
/////////////////////////////////////////////////////////////////

#include "ITranslator.h"
#include "../NativeCpp/INativeCpp.h"

using namespace System;
using namespace System::Text;

public ref class Translator : ITranslator
{
public: 
  Translator()
  {
    pNativeCpp_ = createNativeCpp();
  }
  ~Translator()
  {
    delete pNativeCpp_;
    pNativeCpp_ = nullptr;
  }
  void putString(String^ str) override
  {
    pNativeCpp_->putString(mToN(str));
  }
  String^ getString() override
  {
    return nToM(pNativeCpp_->getString());
  }
  virtual List<double>^ convertToSquares(List<double>^ dbls) override
  {
    return nToM(pNativeCpp_->convertToSquares(mToN(dbls)));
  }
private:
  std::string mToN(String^ str);
  String^ nToM(const std::string& str);
  std::vector<double> mToN(List<double>^ listDbls);
  List<double>^ nToM(const std::vector<double>& vecDbls);
  INativeCpp* pNativeCpp_ = nullptr;
};

ITranslator^ TranslatorFactory::createTranslator()
{
  return gcnew Translator();
}

std::string Translator::mToN(String^ str)
{
  std::string temp;
  for (int i = 0; i < str->Length; ++i)
    temp += char(str[i]);
  return temp;
}

String^ Translator::nToM(const std::string& str)
{
  StringBuilder^ sb = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    sb->Append((wchar_t)str[i]);
  return sb->ToString();
}

std::vector<double> Translator::mToN(List<double>^ dbls)
{
  std::vector<double> temp;
  for (int i = 0; i < dbls->Count; ++i)
    temp.push_back(dbls[i]);
  return temp;
}

List<double>^ Translator::nToM(const std::vector<double>& dbls)
{
  List<double>^ temp = gcnew List<double>();
  for (size_t i = 0; i < dbls.size(); ++i)
    temp->Add(dbls[i]);
  return temp;
}

#include <iostream>

int main(array<System::String ^> ^args)
{
  ITranslator^ tran = TranslatorFactory::createTranslator();
  tran->putString("hello CSE687");
  Console::Write("\n  {0}\n", tran->getString());
  List<double>^ dbls = gcnew List<double>();
  dbls->Add(1.0);
  dbls->Add(1.5);
  dbls->Add(2.0);
  List<double>^ sqrs = tran->convertToSquares(dbls);
  Console::Write("\n  ");
  for (int i = 0; i < sqrs->Count; ++i)
    Console::Write("{0} ", sqrs[i]);
  Console::Write("\n\n");
  return 0;
}
