CPLUS_INCLUDE_PATH = /usr/local/boost_1_55_0:../../../Lib/ExternalLib/tinyxml2:../../../Lib/ServerNetworkLib:../../Packet
export CPLUS_INCLUDE_PATH

VPATH = ../../../Lib/ExternalLib/tinyxml2 ../../../Lib/ServerNetworkLib ../../Packet .

OBJECTS = tinyxml2.o Log.o WorkerThread.o Network.o IOManager.o ServerInfo.o FrontData.o FrontServer.o User.o main.o

FrontServer : $(OBJECTS)
	g++ -g -rdynamic -o $@ $^ -L/usr/local/boost_1_55_0/stage/lib -luuid -lz -lboost_filesystem -lboost_system -lpthread
	
clean:
	rm -rf *.o FrontServer