struct COLOR{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};
struct COLOR MAKE_COLOR(int r,int g,int b,int a){
    struct COLOR c = {r,g,b,a};
    return c;
}
struct INTCOLOR {
    int x;
    struct COLOR y;
};
struct VOXEL{
    int x;
    int y;
    int z;
    struct COLOR color;
};

struct LIST_NODE{
    void *data;
    struct LIST_NODE *next_node;
};

struct LIST{
    int length;    
    struct LIST_NODE *starting_node;
};
struct CHUNK{
    int x;
    int y;
    int z;
    struct LIST chunk[BLOCK_MAP_SIZE];
};
struct WORLD {
    struct LIST chunk_map[CHUNK_MAP_SIZE];
};
struct LIST MAKE_LIST() {
    struct LIST L = {0, NULL};
    return L;
}


void FREE_LIST(struct LIST *list){
    struct LIST_NODE *node = list->starting_node;
    for(int i = 0;i<list->length;i++){
        struct LIST_NODE *erased_node = node;
        node = node->next_node;
        free(erased_node->data);
        free(erased_node);
    }
    

}

void PUSH(struct LIST *list, void *data, size_t data_size){
    struct LIST_NODE* new_node = (struct LIST_NODE*) malloc(sizeof(struct LIST_NODE));
    new_node->data = malloc(data_size);
    new_node->next_node = list->starting_node;
    
    memcpy(new_node->data, data, data_size);
    
    list->starting_node = new_node;
    list->length++;
}

int HASH(int x, int y, int z, int remainder){
    return REMAINDER((x*35317 + y*33391 + z*27361),remainder);
}
void ADD_VOXEL(struct WORLD *w, int x, int y, int z,struct VOXEL *v){
    struct LIST *l = &(w->chunk_map[HASH(x,y,z,CHUNK_MAP_SIZE)]);
    struct LIST_NODE *node = l->starting_node;
    bool found = 0;
    for(int i = 0;i<l->length && found == 0;i++){
        struct CHUNK *data = node->data;
        if(data->x==x && data->y == y && data->z==z){
            found = 1;
            break;
        }
        node = node->next_node;
    }
    if (found == 0){
        struct CHUNK emptychunk = {x,y,z,MAKE_LIST()};
        PUSH(l,&emptychunk,sizeof(emptychunk));
    }
    node = l->starting_node;
    for(int i = 0;i<l->length;i++){
        struct CHUNK *data = node->data;
        if(data->x==x && data->y == y && data->z==z){
            PUSH(&(data->chunk[HASH(v->x,v->y,v->z,BLOCK_MAP_SIZE)]),v,sizeof(struct VOXEL));
            return;
        }
        node = node->next_node;
    }
}
struct INTCOLOR GET_TEXTURE(struct WORLD *W, int x, int y, int z, int vx, int vy, int vz){
    struct LIST *l = &(W->chunk_map[HASH(x,y,z,CHUNK_MAP_SIZE)]);
    struct LIST_NODE *node = l->starting_node;
    bool found = 0;
    for(int i = 0;i<l->length && found == 0;i++){
        struct CHUNK *data = node->data;
        if(data->x==x && data->y == y && data->z==z){
            found = 1;
            struct LIST *chunk_l = &(data->chunk[HASH(vx,vy,vz,BLOCK_MAP_SIZE)]);
            struct LIST_NODE *chunk_node = chunk_l->starting_node;
            for(int j = 0;j<chunk_l->length;j++){
                struct VOXEL *data = chunk_node->data;
                if(data->x == vx && data->y == vy && data->z == vz){
                    struct INTCOLOR ret = {1,data->color};
                    return ret;
                }
                chunk_node = chunk_node->next_node;
            }
        }
        node = node->next_node;
    }
    struct INTCOLOR ret = {0,MAKE_COLOR(0,0,0,0)};
    return ret;
}