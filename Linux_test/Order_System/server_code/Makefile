db_test:db_test.cpp db.hpp
	gcc $^ -o $@  -L /usr/lib64/mysql -lmysqlclient -ljsoncpp -lpthread -g 

.PHONY:clean
clean:
	rm db_test
