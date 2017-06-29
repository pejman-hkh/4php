opencv.so: opencv.o
	$(CXX) -shared $^ -o $@ -Wl,--whole-archive `pkg-config --cflags --libs opencv` -Wl,--no-whole-archive

opencv.o : CXXFLAGS+=-fPIC -std=c++11 -I../../