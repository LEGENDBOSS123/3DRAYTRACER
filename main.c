#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "variables.h"
#include "helperfunctions.h"
#include "vectors.h"
#include "player.h"
#include "structs.h"
#include "raytracer.h"



void SET_PIXEL(unsigned char *PIXELS,int x,int y, Uint8 color[3],int size){
    
    color[0] = pow((float)color[0]/255,0.4545)*255;
    color[1] = pow((float)color[1]/255,0.4545)*255;
    color[2] = pow((float)color[2]/255,0.4545)*255;
    

    for(int Y = y;Y<y+size && Y<SCREEN_HEIGHT;Y = Y + 1){
        for(int X = x;X<x+size && Y<SCREEN_WIDTH;X = X + 1){
            PIXELS[(X+Y*SCREEN_WIDTH)*4] = color[0];
            PIXELS[(X+Y*SCREEN_WIDTH)*4+1] = color[1];
            PIXELS[(X+Y*SCREEN_WIDTH)*4+2] = color[2];
            PIXELS[(X+Y*SCREEN_WIDTH)*4+3] = 255;
        }
        
    }
}


int main() { 
    
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window *WINDOW = SDL_CreateWindow("VOXEL GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Texture *TEXTURE = SDL_CreateTexture(RENDERER,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH,SCREEN_HEIGHT);
    
    unsigned char *PIXELS;
    int PITCH;
    
    Uint32 OTIME = 0;
    Uint32 NTIME = 0;
    double FPS = 0;
        
    const Uint8 *KEYSTATE = SDL_GetKeyboardState(NULL);
    if (KEYSTATE == NULL) {
        return -1;
    }
    
    
    //float x, float y,float z,int renderdis, float ax, float ay, float fov, float grav,float scroll,float speed, float jump
    struct PLAYER P = MAKE_PLAYER(0,10,0,32,0,0,90,0.5,5,2,1);
    struct WORLD W;
    
    for(int i = 0;i < CHUNK_MAP_SIZE; i++){
        W.chunk_map[i] = MAKE_LIST();
    }
    struct VOXEL v;
    int i = 0;
    int j = 0;
    int k = 0;
    int xpos = 0;
    int ypos = 0;
    int zpos = 0;
    for(i = 0; i<128; i+=1){
        for(j = 0; j<10; j+=1){
            for(k = 0; k<128; k+=1){
                xpos = i;
                ypos = i*k*j/100;
                zpos = k;
                v.x = REMAINDER(xpos,CHUNK_SIZE);
                v.y = REMAINDER(ypos,CHUNK_SIZE);
                v.z = REMAINDER(zpos,CHUNK_SIZE);
                v.color = MAKE_COLOR(xpos,xpos*zpos/16,zpos,255);
                if(ypos<0){
                    v.color = MAKE_COLOR(255,0,0,255);
                }
                
                ADD_VOXEL(&W,(int)floor((double)xpos/(double)CHUNK_SIZE),(int)floor((double)ypos/(double)CHUNK_SIZE),(int)floor((double)zpos/(double)CHUNK_SIZE),&v);

            }
        
        }
        
    }
    

    
    

    SDL_Event EVENT;
    
    bool RUNNING = true;
    
    int COUNTER = 0;
    float oldx;
    float oldy;
    float oldz = (float)P.calculation;
    float newx;
    float newy;
    float newz;
    float normalization;
    float direction[3];
    
    Uint8 color[3]={0,0,0};
   
    
    while (RUNNING){
        OTIME = SDL_GetTicks();
        
        HANDLE_KEYPRESSES(KEYSTATE,&P);
        HANDLE(&P);
        
       
        
        
        
        
        SDL_LockTexture(TEXTURE,NULL,(void**)&PIXELS,&PITCH);
        while(SDL_PollEvent(&EVENT)){
            
            if(EVENT.type == SDL_QUIT)
            {
                RUNNING = false;
            }
        }
        
        float sx = sin(PI/180*P.anglex);
        float cx = cos(PI/180*P.anglex);
        
        float sy = sin(PI/180*P.angley);
        float cy = cos(PI/180*P.angley);
        
        int middlex = SCREEN_WIDTH/2;
        int middley = SCREEN_HEIGHT/2;
        
        
        for(int y = 0; y < SCREEN_HEIGHT; y = y + (int)RESOLUTION_SIZE) {
            for(int x = 0; x < SCREEN_WIDTH; x = x + (int)RESOLUTION_SIZE) {
                
                oldx = (float)(x-middlex)/SCREEN_WIDTH;
                oldy = (float)(middley-y)/SCREEN_WIDTH;
                newy = (float)oldy*cy - oldz*sy;
                newz = (float)oldz*cy + oldy*sy;
                newx = (float)oldx*cx - newz*sx;
                newz = (float)newz*cx + oldx*sx;
                normalization = (float) sqrtf(newx*newx+newy*newy+newz*newz) + 1e-6;
                newx=(float)newx/normalization;
                newy=(float)newy/normalization;
                newz=(float)newz/normalization;

                direction[0] = (float)newx;
                direction[1] = (float)newy;
                direction[2] = (float)newz;
                
                RAYTRACE(color,&W,&P,direction);
                
                SET_PIXEL(PIXELS,x,y,color,(int)RESOLUTION_SIZE);
                
            
            
            }
        }
        
        
        
        SDL_RenderClear(RENDERER);
        SDL_UnlockTexture(TEXTURE);
        SDL_RenderCopy(RENDERER,TEXTURE,NULL,NULL);
        SDL_RenderPresent(RENDERER);
        if(COUNTER>50){
            
            NTIME = SDL_GetTicks();
            FPS = 1000/(NTIME-OTIME);
            printf("%i FPS\n",(int)FPS);
            COUNTER = 0;
        }
        
        COUNTER++;
        
    }
    for(int i = 0;i < CHUNK_MAP_SIZE; i++){
        FREE_LIST(&W.chunk_map[i]);
    }
    SDL_DestroyWindow(WINDOW);
    SDL_DestroyRenderer(RENDERER);
    SDL_Quit();

    return 0;
}

