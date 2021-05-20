File=Snake_game
Lib=-lcurses -lpthread

Snake:snake.o
	gcc snake.o -o $(File) $(Lib)
snake.o:snake.c
	gcc -c snake.c
clean:
	rm -rf *.o
	rm -rf $(File)
