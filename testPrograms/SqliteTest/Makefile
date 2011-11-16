OBJS = sqlite3.o MoSyncDB.o dbtest.o

dbtest: $(OBJS)
	g++ -o dbtest $(OBJS)

sqlite3.o: sqlite3.c sqlite3.h
	gcc -c sqlite3.c

MoSyncDB.o: MoSyncDB.cpp MoSyncDB.h sqlite3.h
	g++ -c MoSyncDB.cpp

dbtest.o: dbtest.cpp MoSyncDB.cpp MoSyncDB.h
	g++ -c dbtest.cpp
