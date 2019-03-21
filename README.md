## The aim of this project is to implement FTP application between a server and a client. The application is a console based application and is implemented using Unix Sockets. The project is developed in C/C++ language.
## File Transfer Protocol (FTP) is an application layer protocol which moves files between local and remote file systems. When a FTP session is started between a client and a server, the client initiates a TCP connection with the server side. The client has to do FTP login, FTP login uses normal username and password scheme for granting access. The username is sent to the server using the USER command, and the password is sent using the PASS command. If the information provided by the client is accepted by the server, the server will send a greeting to the client and the session will commence. 
You have to implement following FTP Commands:
- USER – This command sends the user identification to the server.
- PASS – This command sends the user password to the server.
- MKD – This command causes the directory specified in the path name to be created as a directory.
- CWD – This command allows the user to change the current working directory to the path specified with the command.
- RMD – This command causes the directory specified in the path-name to be removed as a directory.
- PWD – This command causes the name of the current working directory to be returned in the reply.
- RETR – This command send the requested file over the data connection.
- STOR – This command causes to store a file into the current directory of the remote host.
- LIST – Sends a request to display the list of all the files present in the directory.
- ABOR – This command tells the server to abort the previous FTP service command and any associated transfer of data.
- QUIT – This command terminates a USER and if file transfer is not in progress, the server closes the connection.
- FTP Replies – Some of the FTP replies are:
```
200	Command okay.
530 	Not logged in.
331 	User name okay, need password.
225 	Data connection open; no transfer in progress.
551 	Requested action aborted: page type unknown.
502 	Command not implemented.
503 	Bad sequence of commands.
504 	Command not implemented for that parameter.
```
