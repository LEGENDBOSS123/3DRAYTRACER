struct PLAYER {
    float x;
    float y;
    float z;
    float render_distance;
    float anglex;
    float angley;
    float fov;
    float xvel;
    float yvel;
    float gravity;
    float scroll_speed;
    float speed;
    float jump;
    float calculation;
    
};


struct PLAYER MAKE_PLAYER(float x, float y,float z, int r, float ax, float ay, float fov, float grav,float s,float s1,float jump){
    struct PLAYER P = {x,y,z,r,ax,ay,fov,0,0,grav,s,s1,jump,tan((PI*(180-fov))/360)/2};
    return P;
}


void HANDLE(struct PLAYER *P){
    
    P->x+= P->xvel;
    
    P->y+= P->yvel;
    
    if(P->angley>=90){
        P->angley=90;
    }
    else if(P->angley<=-90){
        P->angley=-90;
    }
    if(P->anglex>=180){
        P->anglex=-180;
    }
    else if(P->anglex<=-180){
        P->anglex=180;
    }

}
void HANDLE_KEYPRESSES(const Uint8 *K,struct PLAYER *P){
    
    if(K[SDL_SCANCODE_RIGHT]){
        P->anglex -= P->scroll_speed;
    }
    else if(K[SDL_SCANCODE_LEFT]){
        P->anglex += P->scroll_speed;
    }
    if(K[SDL_SCANCODE_UP]){
        P->angley -= P->scroll_speed;
    }
    else if(K[SDL_SCANCODE_DOWN]){
        P->angley += P->scroll_speed;
    }
    float sx = sin(P->anglex*PI/180);
    float cx = cos(P->anglex*PI/180);
    
    float sxR = sin(PI/180*(P->anglex+90));
    float cxR = cos(PI/180*(P->anglex+90));
    
    float sxL = sin(PI/180*(P->anglex-90));
    float cxL = cos(PI/180*(P->anglex-90));
    
    if(K[SDL_SCANCODE_D]){
        P->z -= P->speed*cxR;
        P->x += P->speed*sxR;
    }
    else if(K[SDL_SCANCODE_A]){
        P->z -= P->speed*cxL;
        P->x += P->speed*sxL;
    }
    if(K[SDL_SCANCODE_W]){
        P->z += P->speed*cx;
        P->x -= P->speed*sx;
    }
    else if(K[SDL_SCANCODE_S]){
        P->z -= P->speed*cx;
        P->x += P->speed*sx;
    }
    if(K[SDL_SCANCODE_SPACE]){
        P->y += P->speed;
    }
    else if(K[SDL_SCANCODE_LSHIFT]){
        P->y -= P->speed;
    }
    
    if(K[SDL_SCANCODE_K] && RESOLUTION_SIZE < 50){
        RESOLUTION_SIZE += 0.1;
    }
    else if(K[SDL_SCANCODE_L] && RESOLUTION_SIZE>1){
        RESOLUTION_SIZE -= 0.1;
    }
    else if(K[SDL_SCANCODE_H]){
        RESOLUTION_SIZE = 4;
    }
    else if(K[SDL_SCANCODE_B]){
        RESOLUTION_SIZE = 1;
    }
    else if(K[SDL_SCANCODE_N] && RESOLUTION_SIZE>1){
        RESOLUTION_SIZE = 8;
    }

}

