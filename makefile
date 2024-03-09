# Copyright 2024 Chris Taks

# uses g++ compiler
cc = g++

# path for include directive and linger
path = ..

# compile with C++ 20 standard
standard = -std=c++2a

flags += -Wall
flags += -pedantic
flags += -I $(path)
flags += -g

link = $(cc) $(flags) -o

compile = $(cc) $(flags) -c -o

all: client server

client : client.o
	$(link) $@ $^

client.o : client.cc
	$(compile) $@ $<

server: server.o
$(link) $@ $^

server.o : server.cc
	$(compile) $@ $<

clean: 
	$(RM) *.o $(all)
