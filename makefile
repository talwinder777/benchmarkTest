
benchmark:driver.cpp
	g++ -pthread driver.cpp -o benchmark

clean:
	rm benchmark
