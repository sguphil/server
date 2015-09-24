# server
This server is used to build a mmorpg game server or other high performance server,it provides many tools to make you easily to build a light weight network server.
The server is only support linux platform.

#How To run the Test?
Now, the server provided a client-server test.
The test include three parts: the account-server, test-client, db-server, about the db-server, we only support mysql now.

Runing the test server and client you can follow the steps,as usual you don't need to modify the configurations of the test-servers.
1.clone the server source code
2.install mysql and  create database
    after installed the database, create a database and modify the config file DBSvr.xml(directory "server/DBSvr/config"), set "<dbname>" to your database name.
    run testsql.sql(directory "server/database") to create a test table;
3.install protocol buf
4.compile the db-server
    cd DBSvr
    make clean
    make
    ./dbsvr
5.compile the Account-server
    cd AccSvr
    make clean
    make
    ./accsvr
6.compile the test-client
    cd testclient
    make clean
    make
    ./strictclient

In the client-server test, the terminal will print the info of running procedure,the test-client constantly
send a test package to account-server, when account-server recieve the package, it send a package to db-server,and then the db-server deserialization the msg and search for the user in database by username and password.

In the following days, i will bring protobuf and redis to the server as the package 
serialization/deserialization method,and the cache.

简要说明：
    本服务器提供了一个轻量级网络服务器框架，里面提供了服务器中常用的工具及一些接口的封装，使用这个服务目的在于更容易的搭建起一个可用的高性能的网络服务器，目前这个服务器还需要很多地方需要完善，在服务器性能方面仍然有很大的提升空间，并且服务器只提供linux环境的支持。
    本服务器目前提供了一个简单的测试例子，这个例子是以当前手游服务器常用框架为背景，这个测试提供了一个测试客户端，账号服务器，数据库服务器；例子演示了客户端不断的发账号和密码到账号服务器，账号服务器转包到数据库服务器进行账号密码验证并把结果返回。
    这个服务器中易用之处在于，你可以把账号服务器改造成你想要实现的服务器，在其中加入业务逻辑。由于这个服务器采用了命令式的交互方式，所以你只需要定义好协议字然后注册对应的处理函数即可。
    在使用这个服务器需要关注的几个点是：
    1.当服务器启动无论是集群还是接收客户端连接的第一个数据包必然是一个携带自身服务器类型的二进制数据包。
    2.除了第一个数据包使用二进制进行服务器注册外，我们所有的数据包都将使用protocolbuf，但凡有协议都需要定义在"server/protocol/proto" 目录（当然你可以修改这个目录），然后运行protoc.sh编译出对应的源码（你需要先安装protobuf），在使用的时候包头文件。
    3.每一个服务器都会在其服务器下有include和src目录，里面分别有"*HandlerFunc.hpp"、"*HandlerFunc.cpp"和"*SessionHandler.hpp"、"*SessionHandler.cpp"."*SessionHandler.hpp"文件中addAllHandle函数使用协议号注册你需要的处理函数，在"*HandlerFunc.%pp"文件进行处理函数的声明和实现。
    使用这服务器，你可以简单的在上面的处理函数管理器中注册和实现函数就可以使用服务器了。单如果你需要实现集群的话，这里就需要一套集群的策略来实现负载均衡以及不同进程间的数据共享问题，本服务器后期将会使用redis来实现不同进程间的数据共享，最终实现一个可扩展的、高可用的分布式服务器。
    

