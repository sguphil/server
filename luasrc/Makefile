
LIB = llib.so
$(LIB):
        gcc -fPIC -llua -lm -shared -o $@ luaTest.hpp
        g++  -o fun -g fun.cpp -llua -lm -ldl
        gcc  -o luaMain -g luaTestMain.cpp -llua -lm -ldl

