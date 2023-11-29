#include "ctermui/ctermui.h"
#include <string.h>


int main(){
    
    ctermui_frame root = ctermui_create_frame("frame_1");
    ctermui_frame c1 = ctermui_create_frame("frame_2");
    ctermui_frame c2 = ctermui_create_frame("frame_3");
    ctermui_add_frame(c1,root);
    ctermui_add_frame(c2,root);

    for (size_t i = 0; i < 100; i++)
    {
        if(root->children[i] == NULL){
            break;
        }
        printf("%s ", root->children[i]->Name);
    }
    

    return 0;
}   