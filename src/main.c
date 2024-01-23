#include <stdio.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define ON_CELL  0x2588 
#define OFF_CELL 0x0020 //0x2591

#define WIDTH  1
#define HEIGHT 8

//#define __USE_POSIX199309

struct tm *tm;

typedef struct tframe {
    uint8_t data[(WIDTH * HEIGHT)];
} Frame;

void clearTerm() {
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    printf("%s", CLEAR_SCREEN_ANSI);
}

void initTerm(){
    clearTerm();
    setlocale(LC_CTYPE, "");
}

void printByte(uint8_t byte) {
    for(int i = 0; i < 8; ++i){
        if(((byte >> i) & 0x01) == 1) {
            printf("%lc%lc", ON_CELL, ON_CELL);
        } else {
            printf("%lc%lc", OFF_CELL, OFF_CELL);
        }
    }
}

void printFrame(Frame *frame){
    for(int i = 0; i < HEIGHT; i++){
        printByte(frame->data[i]);
        printf("\n");
    }
}

time_t getTime(){
    
    time_t now;
    now = time(0);

    if ((tm = localtime (&now)) == NULL){
        printf("Error extracting time stuff\n");
        _exit(1);
    }
    return now;
}

int main(){
    initTerm();
    //time_t sec = getTime();
    //printf("time: %ld\n", sec);


    Frame f1 = {
        .data = {
            0x40,
            0xe0,
            0x40,
            0x0,
            0x0,
            0x2,
            0x7,
            0x2,
        }
    };
    Frame f2 = {
        .data = {
            0x2,
            0x7,
            0x2,
            0x0,
            0x0,
            0x40,
            0xe0,
            0x40,
        }
    };
    long count = 0;
    //printFrame(&f1);
    bool x = true;
    struct timespec remaining, request = {1, 100};
    for(;;){
        
        clearTerm();
        if(x){
            printFrame(&f1);
        } else {
            printFrame(&f2);
        }
        x = !x;
        nanosleep(&request, &remaining);
    }
   
    return 0;
}