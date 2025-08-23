terminal: chessTerminal

DS: chessDS

chessTerminal: chess.o terminalGraphics.o main.o
	gcc chess.o terminalGraphics.o main.o -o chessTerminal.exe
	rm chess.o terminalGraphics.o main.o

chessDS: chess.o DSGraphics.o main.o
	gcc chess.o DSGraphics.o main.o -o chessDS.exe
	rm chess.o DSGraphics.o main.o

chess.o: chess.c
	gcc -c chess.c

terminalGraphics.o: terminalGraphics.c
	gcc -c terminalGraphics.c

DSGraphics.o: DSGraphics.c
	gcc -c DSGraphics.c

main.o: main.c
	gcc -c main.c

clean:
	rm chess.o terminalGraphics.o DSGraphics.o main.o chessTerminal.exe chessDS.exe
