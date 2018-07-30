CPP = g++

FLAGS = -std=c++11 -O3

simulation :
	$(CPP) $(FLAGS) -c simulation.cpp

csv_reader :
	$(CPP) $(FLAGS) -c csv_reader.cpp

params :
	$(CPP) $(FLAGS) -c params.cpp

clock :
	$(CPP) $(FLAGS) -c clock.cpp

node :
	$(CPP) $(FLAGS) -c node.cpp

pop :
	$(CPP) $(FLAGS) -c pop.cpp

link :
	$(CPP) $(FLAGS) -o simulation csv_reader.o params.o clock.o node.o simulation.o pop.o

all : csv_reader params clock node pop simulation link

clean :
	rm -f simulation simulation.o params.o node.o csv_reader.o pop.o clock.o