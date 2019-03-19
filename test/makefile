unittest: main.o CountDownLatch.o
	g++ -o unittest main.o CountDownLatch.o -lpthread

main.o: main.cc
	g++ -c -o main.o -lpthread main.cc

CountDownLatch.o: ../base/CountDownLatch.cc
	g++ -c -o CountDownLatch.o -lpthread ../base/CountDownLatch.cc

clean:
	rm CountDownLatch.o main.o
