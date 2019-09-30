#Makefile

CXXFLAGS += --std=c++17

main: se_dictionary.o
	$(CXX) $(CXXFLAGS) -o run se_dictionary.o

se_dictionary.o: se_dictionary.c 
	$(CXX) $(CXXFLAGS) -c se_dictionary.c 

clean: 
	rm *.o run