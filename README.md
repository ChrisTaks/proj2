# CSCE 311 Project 2: IPC
## Chris Taks

This project consists of two programs, a client and a server.
the client will take the arguments: socket name, file name, 1,...,n integers. 
The integers will be the lines from the file you want to read from.
The two files to choose from are located in ./proj2/dat. Each line is an equation.
The client will connect to the socket provided and send the rest of the arguments to the server (assuming the server is already started and contains the same socket name). The server will then take in the arguments, attempt to open the file (will error out if given in invalid file name), and send the client all the equations that it requested.
The client will then process those equations (using the code from project 1, altered a bit to fit this project) and then print those equations, along with their computed values, to the command line.
Once the client has done that, it will then exit and the server will close the socket.

The domain_socket.cc file provides back end support that both the client and the server use. 

All 3 files ending in .cc have a .h header file to support the functions that they run.

A makefile is also included to compile the source files.
`make server` compiles the server.
`make client` compiles the client.
