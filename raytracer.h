void RAYTRACE(Uint8 c[3],struct WORLD *W, struct PLAYER *P, float DIRECTION[3]){
    int voxel_size = 1;
    float ray_point[3];
    ray_point[0] = P->x;
    ray_point[1] = P->y;
    ray_point[2] = P->z;
    
    int currentX = ceil(ray_point[0]/voxel_size);
    int currentY = ceil(ray_point[1]/voxel_size);
    int currentZ = ceil(ray_point[2]/voxel_size);
    
    int stepX = 0;
    int stepY = 0;
    int stepZ = 0;
    float tMaxX = tMax;
    float tMaxY = tMax;
    float tMaxZ = tMax;
    float tDeltaX = tMax;
    float tDeltaY = tMax;
    float tDeltaZ = tMax;
    
    if (DIRECTION[0]<0){
        stepX = -1;
        tDeltaX = stepX*voxel_size/DIRECTION[0];
        tMaxX = ((currentX-1)*voxel_size-ray_point[0])/DIRECTION[0];
    }
    else if (DIRECTION[0]>0){
        stepX = 1;
        tDeltaX = stepX*voxel_size/DIRECTION[0];
        tMaxX = (currentX*voxel_size-ray_point[0])/DIRECTION[0];
    }
   
    
    
    if (DIRECTION[1]<0){
        stepY = -1;
        tDeltaY = stepY*voxel_size/DIRECTION[1];
        tMaxY = ((currentY-1)*voxel_size-ray_point[1])/DIRECTION[1];
    }
    else if (DIRECTION[1]>0){
        stepY = 1;
        tDeltaY = stepY*voxel_size/DIRECTION[1];
        tMaxY = (currentY*voxel_size-ray_point[1])/DIRECTION[1];
    }
    
    
    
    if (DIRECTION[2]<0){
        stepZ = -1;
        tDeltaZ = stepZ*voxel_size/DIRECTION[2];
        tMaxZ = ((currentZ-1)*voxel_size-ray_point[2])/DIRECTION[2];
    }
    else if (DIRECTION[2]>0){
        stepZ = 1;
        tDeltaZ = stepZ*voxel_size/DIRECTION[2];
        tMaxZ = (currentZ*voxel_size-ray_point[2])/DIRECTION[2];
    }
    
    float distance_traveled = 0;
    struct INTCOLOR voxel_in;
    //EEEEEEEEE
    c[0] = bg[0];
    c[1] = bg[1];
    c[2] = bg[2];
    //EEEEEEEEE
    while (distance_traveled < P->render_distance){
        voxel_in = GET_TEXTURE(W,(int)floor((double)currentX/(double)CHUNK_SIZE),(int)floor((double)currentY/(double)CHUNK_SIZE),(int)floor((double)currentZ/(double)CHUNK_SIZE),REMAINDER(currentX,CHUNK_SIZE),REMAINDER(currentY,CHUNK_SIZE),REMAINDER(currentZ,CHUNK_SIZE));
        if (voxel_in.x){
            //EEEEEEEEEEE
            Uint8 tempcolor[3];
            float fog = 3;
            float distancetoblock = (pow(P->render_distance,fog)-pow(distance_traveled,fog))/(pow(P->render_distance,fog));
            tempcolor[0] = voxel_in.y.r;
            tempcolor[1] = voxel_in.y.g;
            tempcolor[2] = voxel_in.y.b;
            c[0] = (Uint8)(c[0]+(tempcolor[0]-bg[0])*distancetoblock);
            c[1] = (Uint8)(c[1]+(tempcolor[1]-bg[1])*distancetoblock);
            c[2] = (Uint8)(c[2]+(tempcolor[2]-bg[2])*distancetoblock);            
            //EEEEEEEEEEE
            return;
        }
        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            currentX+=stepX;
            distance_traveled = fabs(tMaxX);
            tMaxX+=tDeltaX;
        }
        else if (tMaxY < tMaxZ ) {
            currentY+=stepY;
            distance_traveled = fabs(tMaxY);

            tMaxY+=tDeltaY;
        }
        else{
            currentZ+=stepZ;
            distance_traveled = fabs(tMaxZ);
            tMaxZ+=tDeltaZ;
        }
        

    }
        
}

