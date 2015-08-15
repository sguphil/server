CC = g++
#1.设定所有源文件搜索目录顺序
vpath % .:../include:../Factory:../network/include:../Thread:../session:../Logic:../Test/include:../protocol:./include

SOURCE = $(wildcard *.cpp ../Factory/*.cpp ../Thread/*.cpp ../network/src/*.cpp ../session/*.cpp ../Logic/*.cpp ../Test/src/*.cpp ../src/*.cpp ./src/*.cpp)

PROBUFSRC = $(wildcard ../protocol/*.cc)

#2.写出所有依赖的obj文件，带相对目录
PROBUFOBJS = $(patsubst %cc, %o, $(PROBUFSRC))
PROBUFOBJS += $(patsubst %cpp, %o, $(SOURCE))
OBJS = $(PROBUFOBJS)

#3.编译选项
CCFLAGS = -Wall -c -g

#4.链接选项及搜索目录
LIB_PATH = /usr/lib
PROBUFPATH = /usr/local/lib
GEN_LIBPATH = /usr/lib64/mysql
LDFLAGS = -L$(LIB_PATH) -L$(GEN_LIBPATH) -L$(PROBUFPATH)
LDLIBS = -ldl -lmysqlclient -lpthread -lprotobuf

#5.生成动态库选项


#静态库选项


#5.可执行目标文件
TARGET = accsvr

#6.依赖推导项
	
%.o:%.cpp
	$(CC) $(CCFLAGS)  $< -o $@
%.o:%.cc
	$(CC) $(CCFLAGS)  $< -o $@
#7.目标文件生成命令项
$(TARGET):$(OBJS)
	$(CC) $(LDFLAGS) -o $@  $(OBJS) $(LDLIBS)

all:$(TARGET)
	
.PHONY:clean
clean:
	rm  $(OBJS) $(TARGET)
