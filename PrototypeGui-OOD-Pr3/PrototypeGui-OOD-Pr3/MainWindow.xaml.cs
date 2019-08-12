/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Prototype for OOD Project #4               //
// ver 1.2                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: left for students
 * - Show Results:  left for students
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 : 28 Mar 2019
 * - fixed bug in DirsMouse_DoubleClick by using try catch block to
 *   handle exception that occurs if user clicks on unpopulated listbox item.
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using Microsoft.VisualBasic;
using System.IO;

namespace PrototypeGui_OOD_Pr4
{
    ///////////////////////////////////////////////////////////////////
    // MainWindow class
    // - Provides navigation and selection to find libraries to test.
    // - Creates a popup window to handle selections.

    public partial class MainWindow : Window
    {
        public string path { get; set; }
        List<string> selectedFiles { get; set; } = new List<string>();
        public SelectionWindow swin { get; set; } = null;
        bool unselecting = false;

        public static WebBrowser wb;
        public static NavigationWindow FrameWindow;

        Shim shim = ShimFactory.createShim();
     
        //Shim shim = ShimFactory.createShim();
        //ITranslator tran = TranslatorFactory.createTranslator();

        public MainWindow()
        {
            InitializeComponent();
        }
        //----< open window showing contents of project directory >------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(4, path);
            LoadNavTab(path);
        }
        //----< find parent paths >--------------------------------------

        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        //----< file Find Libs tab with subdirectories and files >-------

        void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            CurrPath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            
            Dirs.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }

            Files.Items.Clear();
            string[] files = Directory.GetFiles(path);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Files.Items.Add(path + "\\" + name);
            }

            ConvertedFiles.Items.Clear();
            string ConvertedFilePath = path + "\\ConvertedWebPages\\";
            Console.Write("Loading " + ConvertedFilePath);
            string[] HtmlFiles = Directory.GetFiles(ConvertedFilePath);
            CurrPath1.Text = ConvertedFilePath;
            foreach (string f in HtmlFiles)
            {
                ConvertedFiles.Items.Add(System.IO.Path.GetFileName(f));
            }
        }
        //----< handle selections in files listbox >---------------------

        private void Files_SelectionChanged(
          object sender, SelectionChangedEventArgs e
        )
        {
            if (unselecting)
            {
                unselecting = false;
                return;
            }
            if (swin == null)
            {
                swin = new SelectionWindow();
                swin.setMainWindow(this);
            }
            //swin.Show();

            if (e.AddedItems.Count == 0)
                return;
            string selStr = e.AddedItems[0].ToString();
            selStr = System.IO.Path.Combine(path, selStr);
            if (!selectedFiles.Contains(selStr))
            {
                selectedFiles.Add(selStr);
                swin.Add(selStr);
            }
        }
        //----< unselect files called by unloading SelectionWindow >-----

        public void unselectFiles()
        {
            unselecting = true;  // needed to avoid using selection logic
                                 //selectedFiles.Clear();
            Files.UnselectAll();
        }
        //----< move into double-clicked directory, display contents >---

        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            try
            {
                string selectedDir = Dirs.SelectedItem.ToString();
                if (selectedDir == "..")
                    path = getAncestorPath(1, path);
                else
                    path = System.IO.Path.Combine(path, selectedDir);
                LoadNavTab(path);
            }
            catch
            {
                // just return
            }
        }
        //----< shut down the SelectionWindow if open >------------------

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            
            swin.Close();
        }

      
        // ------ Click on CmdLineInput and this function runs ---------------------
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
           
            string str = Interaction.InputBox("\nThe working directory is the same as Path\nPlease Input your command line in the following format:" +
                "/options re1 re2 re3 ..." + "\n\nThen click OK, and you will see all the dependencies in row three " +
                "\n\nThen click Convert to convet all the files and see results in ConvertedFiles tab"
                , "Input Box", "/s Converter.h", -1, -1);

            List<string> CmdLine = new List<string>();
            string[] OptionAndReList = str.Split(' '); //s is the list of regular expressions


            //Console.Write(path);    //Path is the current path
            CmdLine.Add(" ");
            CmdLine.Add(path);
            foreach(var re in OptionAndReList)
            {
                CmdLine.Add(re);
            }
            
            List<String> files = shim.ToBeConverted(CmdLine);
            foreach (String file_ in files)
            {
                FilesToConvert.Items.Add(file_);
            }
        }

        // --------------- Click on convert then the following function runs ----------------
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            if (FilesToConvert.Items.Count != 0)
            {
                shim.convert();
                shim.refineConvertedFiles();
                LoadNavTab(path);
            }
            else
            {
                Console.Write("\nNo files need to be converted now\n");
            }

        }

        private void Files_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = Files.SelectedValue as string;
            //selectedFile = path + "\\" + selectedFile;
            Console.Write(selectedFile);
            if (selectedFile.Contains("html"))
            { 
                Console.Write(selectedFile);
                System.Diagnostics.Process.Start(selectedFile);
            }
            else
            {
                FrameWindow = new NavigationWindow();
                FrameWindow.Source = new Uri(selectedFile);
                FrameWindow.Show();
            }
            /*
            try
            {
                FrameWindow = new NavigationWindow();
                FrameWindow.Source = new Uri(selectedFile);
                FrameWindow.ShowsNavigationUI = false;
                FrameWindow.Show();
            }
            catch (Exception ex){
                string masg = ex.Message;
            }
            */
        }

        private void ConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedFile = ConvertedFiles.SelectedValue as string;
            Console.Write(selectedFile);
            selectedFile = path + "\\ConvertedWebPages\\" + selectedFile;
            Console.Write(selectedFile);
            System.Diagnostics.Process.Start(selectedFile);
            /*try
            {
                FrameWindow = new NavigationWindow();
                FrameWindow.Source = new Uri(selectedFile);
                FrameWindow.ShowsNavigationUI = false;
                FrameWindow.Show();
            }
            catch (Exception ex)
            {
                string masg = ex.Message;
            }*/
        }
    }
}
