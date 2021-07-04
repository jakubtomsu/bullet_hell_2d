	
g++ .\src\engine.cpp .\src\glad.c .\src\input.cpp .\src\graphics.cpp .\src\entity.cpp .\src\game.cpp -I.\lib\include -L.\lib\lib -lglfw3dll -lmingw32 -lopengl32 -lgdi32 -luser32 -lkernel32 -o .\build\game.exe

