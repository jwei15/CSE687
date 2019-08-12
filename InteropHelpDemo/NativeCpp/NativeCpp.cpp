/////////////////////////////////////////////////////////////////
// NativeCpp.cpp - Translates managed calls into native calls  //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019   //
/////////////////////////////////////////////////////////////////
/*
*  This file defines a private class, NativeCpp, that implements the
*  INativeCpp interface.  Clients only need to use the createNativeCpp
*  factory function and the interface pointer it returns.
*/
#include "INativeCpp.h"
#include <iostream>

class NativeCpp : public INativeCpp
{
public:
  virtual void putString(const std::string& str) override
  {
    str_ = str;
  }
  virtual std::string getString() override
  {
    return str_;
  }
  virtual std::vector<double> convertToSquares(const std::vector<double>& vecDble) override
  {
    std::vector<double> squares;
    for (double item : vecDble)
      squares.push_back(item*item);
    return squares;
  }
private:
  std::string str_;
};

///////////////////////////////////////////////////////////
// Factory function for NativeCpp

INativeCpp* createNativeCpp()
{
  return new NativeCpp();
}

///////////////////////////////////////////////////////////
// This simple class doesn't need a test stub
//
//int main()
//{
//    std::cout << "Hello World!\n"; 
//}
