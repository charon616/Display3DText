CXX = g++
CXXFLAGS=`pkg-config opencv --cflags` `pkg-config bullet --cflags` `pkg-config ftgl --cflags`
LDFLAGS = -framework OpenGL -framework GLUT
LDLIBS= -std=c++11 `pkg-config opencv --libs` `pkg-config bullet --libs` `pkg-config ftgl --libs`

Sample: main.cpp main.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o main main.cpp