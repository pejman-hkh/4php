test.so: test.o
	$(CXX) -shared $^ -o $@ -Wl,--whole-archive -Wl,--no-whole-archive

test.o : CXXFLAGS+=-fPIC -std=c++11 -I../../