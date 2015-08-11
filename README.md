# server
a server which is mostly used to build a mmorpg game server

#runing -- 2015.8.11 by myf.
To run this server, now there is no dependences
runing the test server and client you can follow the steps:
cd AccSvr; make clean; make; ./accsvr
cd testclient; make clean; make; ./strictclient

running the test, the terminal will print how much packages are handle in 1sec,from the 
screenshot maybe we can see while the client increasing, the packages were handled for
each client is decreasing.So, maybe we should use another stragedy to advance the network
and working threads.


    In the following days, i will bring protobuf and redis to the server as the package 
serialization/deserialization method,and the cache.
    And now the server does not have a log manager,so i will a bring one.
    As the cluster, i will figure out a stragety to make it easier,and convenient.
    ... ... to be continued .. .. 

