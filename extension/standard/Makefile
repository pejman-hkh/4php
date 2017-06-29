standard.so: standard.o
	$(CXX) -shared $^ -o $@ -Wl,--whole-archive -Wl,--no-whole-archive

standard.o : CXXFLAGS+=-fPIC -std=c++11 -I../../