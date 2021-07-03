 REM g++ -g .\src\engine.cpp -o .\build\game.exe I.\lib\include -L.\lib\lib -lglfw3dll
	
g++ .\src\engine.cpp -I.\lib\include -L.\lib\lib -lglfw3dll -lmingw32 -lopengl32 -lgdi32 -luser32 -lkernel32 -o .\build\game.exe

