//  emcc lib/canvas.c -s WASM=1 -s EXPORTED_FUNCTIONS="['_main','_getCircles']" -o public/canvas.js

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

#define NUM_CIRCLES 20
#define NUM_PARAMS 6

struct Circle
{
    int x;
    int y;
    int r;
    int cr;
    int cg;
    int cb;
};

struct CircleAnimationData
{
    int x;
    int y;
    int r;
    int xv; // velocity x
    int yv; // velocity y
    int xd; // direction x - 1 means forward
    int yd; // direction y - 1 means forward
};

struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

int getRand(max){
    return ( rand() % max );
}

// Init circle data and start render
int main()
{
    srand(time(NULL));

    for(int i=0; i<NUM_CIRCLES; i++){
        int radius = getRand(50);

    
        circles[i].x = getRand(1000) + radius;
        circles[i].y = getRand(1000) + radius;
        circles[i].r = radius;
        circles[i].cr = getRand(255);
        circles[i].cg = getRand(255);
        circles[i].cb = getRand(255);


        animationData[i].x = circles[i].x;
        animationData[i].y = circles[i].y;
        animationData[i].r = radius;
        animationData[i].xv = getRand(10);
        animationData[i].yv = getRand(10);
        animationData[i].xd = 1;
        animationData[i].yd = 1;


    }
    EM_ASM( {render($0, $1);}, NUM_CIRCLES*NUM_PARAMS, NUM_PARAMS); // since 6 data members in each strcut

    //emscripten_run_script("render()");
}

// Return circles to JS

struct Circle * getCircles(canvasWidth, canvasHeight)
{
    //update circle
    for(int i = 0; i < NUM_CIRCLES; i++){
        // collision Right
        if( (animationData[i].x + animationData[i].r) >= canvasWidth )
            animationData[i].xd = -1;

        // collision Left
        if( (animationData[i].x - animationData[i].r) <= 0 )
            animationData[i].xd =  1;

        // collision Bottom
        if( (animationData[i].y + animationData[i].r) >= canvasHeight )
            animationData[i].yd = -1;

        // collision Left
        if( (animationData[i].y - animationData[i].r) <= 0 )
            animationData[i].yd =  1;

        // Move
        animationData[i].x += animationData[i].xv * animationData[i].xd;
        animationData[i].y += animationData[i].yv * animationData[i].yd;

        circles[i].x = animationData[i].x;
        circles[i].y = animationData[i].y;
        
    }
    return circles;
}