////////////////////////////////////////////////////////////////////////////
// NavigatorClient.xaml.cs - Demonstrates Directory Navigation in WPF App //
// ver 2.0                                                                //
// Jim Fawcett, CSE681 - Software Modeling and Analysis, Fall 2017        //
////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines WPF application processing by the client.  The client
 * displays a local FileFolder view, and a remote FileFolder view.  It supports
 * navigating into subdirectories, both locally and in the remote Server.
 * 
 * It also supports viewing local files.
 * 
 * Maintenance History:
 * --------------------
 * ver 2.1 : 26 Oct 2017
 * - relatively minor modifications to the Comm channel used to send messages
 *   between NavigatorClient and NavigatorServer
 * ver 2.0 : 24 Oct 2017
 * - added remote processing - Up functionality not yet implemented
 *   - defined NavigatorServer
 *   - added the CsCommMessagePassing prototype
 * ver 1.0 : 22 Oct 2017
 * - first release
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
using System.IO;
using System.Threading;
using MessagePassingComm;

namespace Navigator
{
    public partial class MainWindow : Window
    {
        private IFileMgr fileMgr { get; set; } = null;  // note: Navigator just uses interface declarations
        private IFileMgr remotefileMgr { get; set; } = null;
        Comm comm { get; set; } = null;
        Dictionary<string, Action<CommMessage>> messageDispatcher = new Dictionary<string, Action<CommMessage>>();
        Thread rcvThread = null;

        public MainWindow(int port)
        {
            InitializeComponent();
            initializeEnvironment();
            Console.Title = "Navigator Client";

            fileMgr = FileMgrFactory.create(FileMgrType.Local); // uses Environment
            getTopFiles();

            comm = new Comm(ClientEnvironment.address, port);
            
            initializeMessageDispatcher();
            rcvThread = new Thread(rcvThreadProc);
            rcvThread.Start();
   
            //initialize remote dirs and files conditions
            remotefileMgr = FileMgrFactory.create(FileMgrType.Remote);
            showRemoteFiles();

            ReloadLocalandRemote();
        }
        //----< make Environment equivalent to ClientEnvironment >-------
        void initializeEnvironment()
        {
            Environment.root = ClientEnvironment.root;
            Environment.address = ClientEnvironment.address;
            Environment.port = ClientEnvironment.port;
            Environment.endPoint = ClientEnvironment.endPoint;
        }

        // ----< To reload the local and remote files and dirs in GUI listbox>---
        void ReloadLocalandRemote()
        {
            showRemoteFiles();
            getTopFiles();
        }


        //----< define how to process each message command >-------------

        void initializeMessageDispatcher()
        {


            messageDispatcher["DownloadConfirmed"] = (CommMessage msg) =>
            {
                Console.Write("Downloading......\n");

                if (msg.arguments[1].Contains('\\'))
                {
                    string[] sArray = msg.arguments[1].Split('\\');
                    File.WriteAllText("../../../ClientFiles/" + comm.getPortNum().ToString() + "/" +
                    sArray[sArray.Length - 1], msg.arguments[0]);

                }
                else
                {
                    File.WriteAllText("../../../ClientFiles/" + comm.getPortNum().ToString() + "/" +
                        msg.arguments[1], msg.arguments[0]);
                }
               
                Console.Write("Download successful\n");
            };


            // ---- <To pop up a remote file > ----
            messageDispatcher["PopUpRemote"] = (CommMessage msg) =>
            {
                CodePopUp popup = new CodePopUp();
                popup.codeView.Text = msg.arguments[0];
                popup.Show();
            };

            // < ---- Receipt of our submission --- >
            messageDispatcher["SubmitConfirmed"] = (CommMessage msg) =>
            {
                Console.Write("Submission successful\n");
                //ReloadLocalandRemote();
            };

            // load remoteFiles listbox with files from root

            messageDispatcher["getTopFiles"] = (CommMessage msg) =>
            {
                remoteFiles.Items.Clear();
                foreach (string file in msg.arguments)
                {
                    remoteFiles.Items.Add(file);
                }
            };
            // load remoteDirs listbox with dirs from root

            messageDispatcher["getTopDirs"] = (CommMessage msg) =>
            {
                remoteDirs.Items.Clear();
                foreach (string dir in msg.arguments)
                {
                    remoteDirs.Items.Add(dir);
                }
            };
            // load remoteFiles listbox with files from folder

            messageDispatcher["moveIntoFolderFiles"] = (CommMessage msg) =>
            {
                remoteFiles.Items.Clear();
                foreach (string file in msg.arguments)
                {
                    remoteFiles.Items.Add(file);
                }
            };
            // load remoteDirs listbox with dirs from folder

            messageDispatcher["moveIntoFolderDirs"] = (CommMessage msg) =>
            {
                remoteDirs.Items.Clear();
                foreach (string dir in msg.arguments)
                {
                    remoteDirs.Items.Add(dir);
                }
            };
        }
        //----< define processing for GUI's receive thread >-------------

        void rcvThreadProc()
        {
            Console.Write("\n  starting client's receive thread");
            while (true)
            {
                CommMessage msg = comm.getMessage();
                msg.show();
                if (msg.command == null)
                    continue;

                // pass the Dispatcher's action value to the main thread for execution

                Dispatcher.Invoke(messageDispatcher[msg.command], new object[] { msg });
            }
        }
        //----< shut down comm when the main window closes >-------------

        private void Window_Closed(object sender, EventArgs e)
        {
            comm.close();

            // The step below should not be nessary, but I've apparently caused a closing event to 
            // hang by manually renaming packages instead of getting Visual Studio to rename them.

            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }
        //----< not currently being used >-------------------------------

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
        }
        //----< show files and dirs in root path >-----------------------

        public void getTopFiles()
        {
            List<string> files = fileMgr.getFiles().ToList<string>();
            localFiles.Items.Clear();
            foreach (string file in files)
            {
                localFiles.Items.Add(file);
            }
            List<string> dirs = fileMgr.getDirs().ToList<string>();
            localDirs.Items.Clear();
            foreach (string dir in dirs)
            {
                localDirs.Items.Add(dir);
            }
        }

        //----< show files that are in remote dir >---
        public void showRemoteFiles()
        {
            if (Directory.Exists("../../../ServerFiles/" + comm.getPortNum().ToString()))
            {
                //do nothing
            }
            else
            {
                Directory.CreateDirectory("../../../ServerFiles/" + comm.getPortNum().ToString());
            }
            List<string> files = remotefileMgr.getFiles().ToList<string>();
            remoteFiles.Items.Clear();
            foreach (string file in files)
            {
                remoteFiles.Items.Add(file);
            }
            List<string> dirs = remotefileMgr.getDirs().ToList<string>();
            remoteDirs.Items.Clear();
            foreach (string dir in dirs)
            {
                remoteDirs.Items.Add(dir);
            }
        }

        //----< move to directory root and display files and subdirs >---

        private void localTop_Click(object sender, RoutedEventArgs e)
        {
            fileMgr.currentPath = "";
            getTopFiles();
        }
        //----< show selected file in code popup window >----------------

        private void localFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string fileName = localFiles.SelectedValue as string;
            try
            {
                string path = System.IO.Path.Combine(ClientEnvironment.root, fileName);
                string contents = File.ReadAllText(path);
                CodePopUp popup = new CodePopUp();
                popup.codeView.Text = contents;
                popup.Show();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        }
        //----< move to parent directory and show files and subdirs >----

        private void localUp_Click(object sender, RoutedEventArgs e)
        {
            if (fileMgr.currentPath == "")
                return;
            fileMgr.currentPath = fileMgr.pathStack.Peek();
            fileMgr.pathStack.Pop();
            getTopFiles();
        }
        //----< move into subdir and show files and subdirs >------------

        private void localDirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string dirName = localDirs.SelectedValue as string;
            fileMgr.pathStack.Push(fileMgr.currentPath);
            fileMgr.currentPath = dirName;
            getTopFiles();
        }
        //----< move to root of remote directories >---------------------
        /*
         * - sends a message to server to get files from root
         * - recv thread will create an Action<CommMessage> for the UI thread
         *   to invoke to load the remoteFiles listbox
         */
        private void RemoteTop_Click(object sender, RoutedEventArgs e)
        {
            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:" + comm.getPortNum().ToString() + "/IMessagePassingComm";
            msg1.to = ServerEnvironment.endPoint;
            msg1.author = "Jim Fawcett";
            msg1.command = "getTopFiles";
            msg1.arguments.Add("");
            comm.postMessage(msg1);
            CommMessage msg2 = msg1.clone();
            msg2.command = "getTopDirs";
            comm.postMessage(msg2);
        }
        //----< download file and display source in popup window >-------

        private void remoteFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:"+comm.getPortNum().ToString() +"/IMessagePassingComm";
            msg1.to = ServerEnvironment.endPoint;
            msg1.author = "Jinhao Wei";
            msg1.command = "PopUpFiles";
            Console.Write(ServerEnvironment.root + remoteFiles.SelectedItem.ToString());
            msg1.arguments.Add(ServerEnvironment.root + remoteFiles.SelectedItem.ToString());
            comm.postMessage(msg1);
        }
        //----< move to parent directory of current remote path >--------

        private void RemoteUp_Click(object sender, RoutedEventArgs e)
        {
            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:" + comm.getPortNum().ToString() + "/IMessagePassingComm";//ClientEnvironment.endPoint;
            msg1.to = ServerEnvironment.endPoint;
            msg1.command = "moveIntoFolderFiles";
            
            if (remotefileMgr.pathStack.Count == 0)
            {
                Console.Write("\nNo peek element at this time\n");
                msg1.arguments.Add("");
            }
            else
            {
                remotefileMgr.pathStack.Pop();
                msg1.arguments.Add(remotefileMgr.pathStack.Peek());
            }
               
            comm.postMessage(msg1);
            CommMessage msg2 = msg1.clone();
            msg2.command = "moveIntoFolderDirs";
            comm.postMessage(msg2);
            // coming soon
        }
        //----< move into remote subdir and display files and subdirs >--
        /*
         * - sends messages to server to get files and dirs from folder
         * - recv thread will create Action<CommMessage>s for the UI thread
         *   to invoke to load the remoteFiles and remoteDirs listboxs
         */
        private void remoteDirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            remotefileMgr.pathStack.Push(remoteDirs.SelectedValue as string);
            remotefileMgr.currentPath = remoteDirs.SelectedValue as string;

            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:" + comm.getPortNum().ToString() + "/IMessagePassingComm";//ClientEnvironment.endPoint;
            msg1.to = ServerEnvironment.endPoint;
            msg1.command = "moveIntoFolderFiles";
            msg1.arguments.Add(remoteDirs.SelectedValue as string);
            comm.postMessage(msg1);
            CommMessage msg2 = msg1.clone();
            msg2.command = "moveIntoFolderDirs";
            comm.postMessage(msg2);
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void DoSubmission(object sender, RoutedEventArgs e)
        {
            string filename = localFiles.SelectedItem.ToString();
            
            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:" + comm.getPortNum().ToString() + "/IMessagePassingComm";//ClientEnvironment.endPoint;
            msg1.to = ServerEnvironment.endPoint;
            msg1.command = "Submit";
            msg1.arguments.Add(filename);  
            msg1.arguments.Add(comm.getPortNum().ToString()); 
            msg1.arguments.Add(File.ReadAllText(ClientEnvironment.root + filename));
            //The arguments look like: filename, portnumber, content of file

            comm.postMessage(msg1); 
        }

        private void Download_Click(object sender, RoutedEventArgs e)
        {
            string filename = remoteFiles.SelectedItem.ToString();
            CommMessage msg1 = new CommMessage(CommMessage.MessageType.request);
            msg1.from = "http://localhost:" + comm.getPortNum().ToString() + "/IMessagePassingComm";//ClientEnvironment.endPoint;
            msg1.to = ServerEnvironment.endPoint;
            msg1.command = "Download";
            msg1.arguments.Add(filename);
            msg1.arguments.Add(comm.getPortNum().ToString());
            comm.postMessage(msg1);
        }
    }
}
