Further implementations are still required for this project

Most of the codes in this repository are starter codes from Professor James Fawcett, Syracuse University(expected to retire May 2019)



====================================================================================================================
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
                                                        
Note: After compile.bat has been run should this run.bat fail(this usually occurs due to some privilege issues), please manually run the project by:
    1. enter the directory 
        NavigatorClientServer/NavigationServer/bin/Debug
        and run NavigatorServer.exe with administrator privilege, 

    2. enter the directory 
        NavigatorClientServer/NavigatorClient/bin/Debug
        and run NavigatorClient.exe with administrator privilege, 
        
    This will do a simple demo for one client and one server case
