///////////////////////////////////////////////////////////////
// CSharpClient.cs - Uses Translator to call NativeCpp       //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019 //
///////////////////////////////////////////////////////////////
/*
 * For a C# client to use a C++\CLI dll, the client needs to
 * make a reference to the C++\CLI project.  If you expand the
 * references link, in the client project, you will see a 
 * reference to Translator.
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpClient
{
  class CSharpClient
  {
    static void Main(string[] args)
    {
      ITranslator tran = TranslatorFactory.createTranslator();
      tran.putString("Hello Interop world");
      Console.Write("\n  {0}\n", tran.getString());

      List<double> dbls = new List<double>();
      dbls.Add(1.0);
      dbls.Add(2.5);
      dbls.Add(4.0);
      List<double> sqrs = tran.convertToSquares(dbls);
      Console.Write("\n  ");
      foreach (var item in sqrs)
        Console.Write("{0} ", item);
      Console.Write("\n\n");
    }
  }
}
