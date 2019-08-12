using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;


namespace Navigator
{
  /// <summary>
  /// Interaction logic for App.xaml
  /// </summary>
  public partial class App : Application
  {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            // Create the startup window
            if (e.Args.Length == 0)
            {
                MainWindow wnd = new MainWindow(9091);
                wnd.Title = ("Client with port: " + "9091");
                wnd.Show();
            }
            else
            {
                MainWindow wnd = new MainWindow(Int32.Parse(e.Args[0]));
                wnd.Title = ("Client with port: " + e.Args[0]);
                wnd.Show();
            }
            
        }
    }
}
