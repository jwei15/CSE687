using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace PrototypeGui_OOD_Pr4
{
  /// <summary>
  /// Interaction logic for App.xaml
  /// </summary>
  public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            
            Shim shim = ShimFactory.createShim();
   
            List<string> CmdLine = new List<string>();
            CmdLine.Add(" ");
            foreach(var str in e.Args)
            {
                CmdLine.Add(str);
                //Console.Write(str);
            }
  
            List<String> files = shim.ToBeConverted(CmdLine);
            shim.convert();
            shim.refineConvertedFiles();
            
        }
    }
}
