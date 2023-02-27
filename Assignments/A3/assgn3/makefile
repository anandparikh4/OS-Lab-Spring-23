main: main.o graph.o producer consumer
	g++ -o main main.o graph.o

main.o: main.cpp graph.h
	g++ -c main.cpp

graph.o: graph.cpp graph.h
	g++ -c graph.cpp

producer: producer.o graph.o
	g++ -o producer producer.o graph.o

producer.o: producer.cpp graph.h
	g++ -c producer.cpp

consumer: consumer.o graph.o
	g++ -o consumer consumer.o graph.o

consumer.o: consumer.cpp graph.h
	g++ -c consumer.cpp

clean:
	rm -rf main main.o consumer consumer.o producer producer.o graph.o

distclean:
	rm -rf main main.o consumer consumer.o producer producer.o graph.o paths_*.txt