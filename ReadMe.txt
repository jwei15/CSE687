This project supports only downloading and uploading

No conversion operations supported!

Please use compile.bat and run.bat to test

Never start this up from Visual Studio 2017
who knows if there will be privilege issues when we are using port



1. The run.bat will invoke three process, one server, two clients

2. The two clients will each invoke a GUI, the title of each of the GUI is the port number of the clients

3. All files uploaded from port 9091 will show its result in 
    Project4_jwei15/NavigatorClientServer/ServerFiles/9091
    
   The same for port 9092
   
5. All files downloaded to port 9091 will show its result in
    Project4_jwei15/NavigatorClientServer/ClientFiles/9091

   The same for port 9092
    
4. Please check Project4_jwei15/NavigatorClientServer/, 
   there are two folders in that directory: ServerFiles and ClientFiles

Project4_jwei15 
       |- - - - - - -- NavigatorClientServer
                               |- - - - - - - - - ClientFiles 
                               |                       | - - 9091
                               |                       | - - 9092
                               |
                               |- - - - - - - - - ServerFiles
                                                        | - - 9091
                                                        | - - 9092