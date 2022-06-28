#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <malloc.h>

#include <windows.h>
#include <conio.h>  

bool deadend(int, int, int**, int, int); // Вспомогательная функция, определяет тупики
void visual(int**, int, int); // Изображение результата
void mazemake(int**, int, int); // Алгоритм генерации
void shroom(int**, int, int); 
void enter_end(int**, int, int);
const int wall = 0, pass = 1, liveshroom  = 2, deadshroom = 3, start = 4, finish = 5;
int enter_x, enter_y, end_x, end_y;

enum ConsoleColor
{
                Black         = 0,
                Green         = 2,
                Red           = 4,
                Yellow        = 14,
                White         = 15
};


void SetColor(int text, int background)
{
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

int main(){
	srand((unsigned)time(NULL));
	
	int height = 101, width = 101;

	int **maze;
	maze = (int**)malloc(height * sizeof(int*));
	for (int i = 0; i<height; i++) {
    	maze[i] = (int*)malloc(width * sizeof(int));
	}
		
	mazemake(maze, height, width);
	enter_end(maze, height, width);
	visual(maze,height,width);
	shroom(maze,height,width);
	visual(maze,height,width);
	for(int i = 0; i < height; i++) 
    free (maze[i]);
    free (maze);
    return 0;
}
bool deadend(int x, int y, int** maze, int height, int width){
	int a = 0;
	
	if(x != 1){
		if(maze[y][x-2] == pass)
			a+=1;
		}
	else a+=1;
	
	if(y != 1){
		if(maze[y-2][x] == pass)
			a+=1;
		}
	else a+=1;
	
	if(x != width-2){
		if(maze[y][x+2] == pass)
			a+=1;
		}
	else a+=1;
	
	if(y != height-2){
		if(maze[y+2][x] == pass)
			a+=1;
		}
	else a+=1;
	
	if(a == 4)
		return 1;
	else
		return 0;
}

void visual(int** maze, int height, int width){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++) {
			switch(maze[i][j]){
				case 0: SetColor(White,Black);printf("0 "); break;
				case 1: SetColor(White,Black);printf("  "); break;
				case 2: SetColor(Green,Black);printf("* "); break;
				case 4: SetColor(Yellow,Black);printf("/ "); break;
				case 5: SetColor(Red,Black);printf(". "); break;
			}
		}
		printf("\n");
		}
	printf("\n");	
}

void mazemake(int** maze, int height, int width){
	int x, y, c, a; 
	bool b;
	
	for(int i = 0; i < height; i++) // Массив заполняется землей-ноликами
		for(int j = 0; j < width; j++)
			maze[i][j] = wall;
	
	x = 3; y = 3; a = 0; // Точка начала и счетчик
	while(a < 10000){
		maze[y][x] = pass; a++;
		while(1){ // Бесконечный цикл, который прерывается только тупиком
			c = rand()%4;
			switch(c){ 
				case 0: if(y != 1)
					if(maze[y-2][x] == wall){ // Вверх
                    maze[y-1][x] = pass;
					maze[y-2][x] = pass;
					y-=2;
				}
				case 1: if(y != height-2)      
					if(maze[y+2][x] == wall){ // Вниз
					maze[y+1][x] = pass;
					maze[y+2][x] = pass;
					y+=2;
				}
				case 2: if(x != 1)
					if(maze[y][x-2] == wall){ // Налево
					maze[y][x-1] = pass;
					maze[y][x-2] = pass;
					x-=2;
				}
				case 3: if(x != width-2)
					if(maze[y][x+2] == wall){ // Направо
					maze[y][x+1] = pass;
					maze[y][x+2] = pass;
					x+=2;
				}
			}
			if(deadend(x,y,maze,height,width))
			   break;
		}
		
		if(deadend(x,y,maze,height,width)) // Выход из тупика
			do{
				x = 2*(rand()%((width-1)/2))+1;
				y = 2*(rand()%((height-1)/2))+1;
			}
			while(maze[y][x] != pass);
	}
}
void shroom(int** maze, int height, int width){
	int x, y;
	
	for(int i = 0; i < height; i++) // Поиск старта
		for(int j = 0; j < width; j++)
			if(maze[i][j] == start){
				x = j; y = i;
			}


	while(maze[y][x] != finish){
		maze[y][x] = liveshroom; 
		if ((x != 0) && (x != (width-1)) && (y != 0) && (y != (height-1)))
		if ((maze[y][x+1] == finish) || (maze[y][x-1] == finish) || (maze[y+1][x] == finish) || (maze[y-1][x] == finish)){
			break;
		}

		if (x != (width-1))
		if(maze[y][x+1] == pass){ 
			maze[y][x+1] = liveshroom;
			x+=1;
			continue;
		}
		if (x != 0)
		if(maze[y][x-1] == pass){ 
			maze[y][x-1] = liveshroom;
			x-=1;
			continue;
		}
		if (y != (height-1))
		if(maze[y+1][x] == pass){
			maze[y+1][x] = liveshroom;
			y+=1;
			continue;
		}
		if (y != 0)
		if(maze[y-1][x] == pass){ 
			maze[y-1][x] = liveshroom;
			y-=1;
			continue;
		}
		if(maze[y][x+1] != pass && 
	       maze[y][x-1] != pass &&  
		   maze[y+1][x] != pass && 
		   maze[y-1][x] != pass){
			maze[y][x] = deadshroom;
			if(maze[y][x+1] == liveshroom){ 
				maze[y][x+1] = deadshroom;
				x+=1;
				continue;
			}
			
			if(maze[y][x-1] == liveshroom){ 
				maze[y][x-1] = deadshroom;
				x-=1;
				continue;
			}
			
			if(maze[y+1][x] == liveshroom){ 
				maze[y+1][x] = deadshroom;
				y+=1;
				continue;
			}
			
			if(maze[y-1][x] == liveshroom){ 
				maze[y-1][x] = deadshroom;
				y-=1;
				continue;
			}
		}
	}
	


	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			if(maze[i][j] == deadshroom)
				maze[i][j] = pass;
}

void enter_end(int** maze, int height, int width) {
	int cout = 0;
	int enter = rand() % (width*2+height*2-4);
	int end = rand() % (width*2+height*2-4);
	int check_enter = 0, check_end = 0;
	int n = 0;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++) {
			if ((i == 0) || (i == height - 1)) {
				if ((cout == enter) || (cout == end)) {
					if (cout == enter) {
						n = 4;
					}
					else {
						n = 5;
					}
					if ((i == 0)) {
					if ((maze[i+1][j] == 1) && (enter != end)) {
						maze[i][j] = n;
						if (cout == enter) {
							check_enter = 1;
							enter_x = j; enter_y = i;
						}
						else {
							check_end = 1;
						}
					}
					}
					if ((i == height - 1)) {
					if (((maze[i-1][j] == 1)) && (enter != end)) {
						maze[i][j] = n;
						if (cout == enter) {
							check_enter = 1;
							enter_x = j; enter_y = i;
						}
						else {
							check_end = 1;
						}
					}
					}
					if ((check_enter != 1) || (check_end != 1)) {
						if (
							(check_enter != 1))
							enter = rand() % (width*2+height*2-4);
						if ((check_end != 1))
							end = rand() % (width*2+height*2-4);
						i = 0; j = 0;
						cout = 0;
					}
				}
				cout++;
			}
			else {
				if ((j == 0) || (j == width - 1)) {
					if ((cout == enter) || (cout == end)){
						if (cout == enter) {
							n = 4;
						}
						else {
							n = 5;
						}
						if ((j == 0)) {
						if ((maze[i][j+1] == 1) && (enter != end)) {
							maze[i][j] = n;
							if (cout == enter) {
								check_enter = 1;
								enter_x = j; enter_y = i;
							}
							else {
								check_end = 1;
							}
						}
						}
						if ((j == (width-1))) {
						if ((maze[i][j-1] == 1) && (enter != end)) {
							maze[i][j] = n;
							if (cout == enter) {
								check_enter = 1;
								enter_x = j; enter_y = i;
							}
							else {
								check_end = 1;
							}
						}
						}
						if ((check_enter != 1) || (check_end != 1)) {
							if ((check_enter != 1))
								enter = rand() % (width*2+height*2-4);
							if ((check_end != 1))
								end = rand() % (width*2+height*2-4);
							i = 0; j = 0;
							cout = 0;
						}
					}
					cout++;
				}
			}
		}
	}
}
