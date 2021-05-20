#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <pthread.h>
#include <unistd.h>

#define UP 1
#define DOWN -1
#define LEFT 2
#define RIGHT -2

#define HIGHT 25
#define WEIGHT 25

struct Snake{
    int H, L;
    struct Snake *next;
};

struct Snake *head=NULL;
struct Snake *tail=NULL;
struct Snake food;

int key;
int direction;
 
void initFood(){
    srand((unsigned)time(NULL));
    int x = rand()%(WEIGHT-1) + 1;
    int y = rand()%(HIGHT-1) + 1;
    food.H = x; 
    food.L = y;  
}

void initCurses(){
    initscr();
    keypad(stdscr,true);
    noecho();
} 

int hasSnakeNode(int x,int y){
    struct Snake *p = head;
    while(p != NULL){
        if(p->H == x && p->L == y){
                return 1;
        }
        p = p->next;
    }
    return 0;
}

int hasFood(int x,int y){
    if(food.H == x && food.L ==y){
       return 1; 
    }
    return 0;
}

void initMap(){
    int x, y;
    move(0,0);
    for(x = 0; x <= WEIGHT; x++){
        if(x == 0){
            for(y = 0; y <= HIGHT; y++){
                printw("[x]");
            }
            printw("\n");
        }
        if(x > 0 && x < WEIGHT){
            for(y = 0; y <= HIGHT; y++ ){
                if(y == 0 || y == HIGHT){
                    printw("[x]");
                }else if(hasSnakeNode(x,y)){
                    printw("(8)");
                }else if(hasFood(x,y)){
                    printw("@#@");
                }else{
                    printw("   ");
                }

            }
            printw("\n");
        }
        if(x == WEIGHT){
            for(y = 0; y <= HIGHT; y++){
                printw("[x]");
            }
                printw("\n");
        }
    }
    printw("The snake food :x = %d,y = %d\n",food.H,food.L);
}

void addNode(){ 
    struct Snake *new;
    new = (struct Snake*)malloc(sizeof(struct Snake));
    new->next = NULL;
    switch(direction){
        case UP:
            new->H = tail->H-1;
            new->L = tail->L;
            break;
        case DOWN:
            new->H = tail->H+1;
            new->L = tail->L;
            break;
        case LEFT:
            new->H = tail->H;
            new->L = tail->L-1;
            break;
        case RIGHT:
            new->H = tail->H;
            new->L =tail->L+1;
            break;
    }
    tail->next = new;
    tail = new;
}

void initSnake(){
    struct Snake *p = NULL;
    initFood();
    direction = RIGHT;
    while(p != NULL){
        p = head;
        free(p);
        p = p->next;
    }
    head = (struct Snake*)malloc(sizeof(struct Snake));
    head->H = 2;
    head->L = 2;
    head->next = NULL;
    tail = head;
    
    addNode();
}

void deleteNode(){
    struct Snake *p;
    p = head;
    head = head->next;
    free(p);
}

int ifSnakeDie(){
    struct Snake *p = head;
    if(tail->H == 0 || tail->H == 25 ||tail->L == 0 ||
            tail->L == 25)
            return 1;
    while(p->next != NULL){
        if(tail->H == p->H && tail->L == p->L){
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void moveSnake(){
    if(tail->H == food.H && tail->L == food.L){
        addNode();
        initFood();
    }
    addNode();
    deleteNode();
    if(ifSnakeDie()){
        initSnake();
    }
}

void turnSnake(int dir){
    if(abs(direction) != abs(dir)){
        direction = dir;
    }
}

void *changeDirection(){
    while(1){
        key = getch();
        switch(key){ 
            case KEY_UP:   turnSnake(UP);   break;
            case KEY_DOWN: turnSnake(DOWN); break; 
            case KEY_LEFT: turnSnake(LEFT); break;
            case KEY_RIGHT:turnSnake(RIGHT);break;
        }
    }
}

void *refreshSnake(){
    while(1){
        moveSnake();
        initMap();
        refresh();
        usleep(200000);
    }
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    initCurses();
    initMap();
    initSnake();

    pthread_create(&t1,NULL,refreshSnake,NULL);
    pthread_create(&t2,NULL,changeDirection,NULL);
    while(1);
    getch();
    endwin();
    return 0;
}
