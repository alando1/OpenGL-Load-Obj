g++ -std=c++11 -static-libstdc++ objLoader.cpp Mesh.cpp SuppliedGlutFuncs.cpp DrawFunc.cpp -o run -O3 -I"./include" -L"./lib"  -lSDL2 -lSDL2main -lSDL2_mixer -lfreeglut -lSOIL -lopengl32 -lglu32 -Wall -Wno-strict-aliasing -Wno-comment

@echo off
echo(

