ipmt:
	g++ src/main.cpp -o $(CURDIR)/bin/ipmt -std=c++14 -O3
add-to-paths: 
	export PATH=${PATH}:$(CURDIR)/bin/
