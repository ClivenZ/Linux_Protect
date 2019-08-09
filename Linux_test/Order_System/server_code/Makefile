FLAG=-L/usr/lib64/mysql -lmysqlclient -ljsoncpp -std=c++11 -lpthread

.PHONY:all
all:server db_test httptest 

server:server.cc  
	g++ $^ -o  $@ $(FLAG)
	gcc -v
db_test:db_test.cpp 
	g++ $^ -o $@ $(FLAG)

httptest:httptest.cc
	g++ $^ -o $@ $(FLAG)  

.PHONY:clean
clean:
	rm db_test server httptest
