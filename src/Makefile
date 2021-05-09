CFLAGS = -std=c++17 -O2
LDFLAGS = -lGL -lglut -lglfw -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

OpenGLTest: main.cpp
	g++ $(CFLAGS) -o OpenGLTest ../glad.c main.cpp $(LDFLAGS)

.PHONY: test clean

test: OpenGLTest
	./OpenGLTest

clean:
	rm -f OpenGLTest

