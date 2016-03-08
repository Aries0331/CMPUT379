#include "factory.h"

const char *colors[] = {"AliceBlue","AntiqueWhite","Aqua","Aquamarine","Azure","Beige","Bisque","Black","BlanchedAlmond","Blue","BlueViolet","Brown","BurlyWood","CadetBlue","Chartreuse","Chocolate","Coral","CornflowerBlue","Cornsilk","Crimson","Cyan","DarkBlue","DarkCyan","DarkGodenRod","DarkGray","DarkGrey","DarkGreen","DarkKhaki","DarkMagenta","DarkOliveGreen","Darkorange","DarkOrchid","DarkRed","DarkSalmon","DarkSeaGreen","DarkSlateBlue","DarkSlateGray","DarkSlateGrey","DarkTurquoise","DarkViolet","DeepPink","DeepSkyBlue","DimGray","DimGrey","DodgerBlue","FireBrick","FloralWhite","ForestGreen","Fuchsia","Gainsboro","GhostWhite","God","GodenRod","Gray","Grey","Green","GreenYellow","HoneyDew","HotPink","IndianRed","Indigo","Ivory","Khaki","Lavender","LavenderBlush","LawnGreen","LemonChiffon","LightBlue","LightCoral","LightCyan","LightGodenRodYellow","LightGray","LightGrey","LightGreen","LightPink","LightSalmon","LightSeaGreen","LightSkyBlue","LightSlateGray","LightSlateGrey","LightSteelBlue","LightYellow","Lime","LimeGreen","Linen","Magenta","Maroon","MediumAquaMarine","MediumBlue","MediumOrchid","MediumPurple","MediumSeaGreen","MediumSlateBlue","MediumSpringGreen","MediumTurquoise","MediumVioletRed","MidnightBlue","MintCream","MistyRose","Moccasin","NavajoWhite","Navy","OdLace","Olive","OliveDrab","Orange","OrangeRed","Orchid","PaleGodenRod","PaleGreen","PaleTurquoise","PaleVioletRed","PapayaWhip","PeachPuff","Peru","Pink","Plum","PowderBlue","Purple","Red","RosyBrown","RoyalBlue","SaddleBrown","Salmon","SandyBrown","SeaGreen","SeaShell","Sienna","Silver","SkyBlue","SlateBlue","SlateGray","SlateGrey","Snow","SpringGreen","SteelBlue","Tan","Teal","Thistle","Tomato","Turquoise","Violet","Wheat","White","WhiteSmoke","Yellow","YellowGreen"};

/** @brief Each packer will extract products one at a time from
 *         the assembly line (the bounded-buffer array).
 *         After retrieving n products, the packer should warn (print to stdout)
 *         the contents of the package
 *
 *  @param A pointer but usually NULL
 *
 *  @return void.
 */

void * packer (void * ptr)
{
    int i,j,k;
    struct item box[n]; // packer box to store the extract products
    
    for(i=0;i<n;i++) {
        
        pthread_mutex_lock(&mutex);
        
        /* check if the assembler line is full, if it is, then wait */
        
        while(numitems!=0 && count == 0) {
            pthread_cond_wait(&cond_p, &mutex);
        }
        
        /* check if all the products have been produced and the assembler line is empty,
           if it is, then print out the contents of the packer and exit */
        
        if(numitems==0 && count == 0) {
            
            /* in case the packer box is not full */
            
            if(i<=n-1 && i != 0) {
                printf("[Packer %04ld]: I have a box of products containing: ",(long)pthread_self()%10000);
                for(k=0;k<i;k++) {
                    printf("%s%d",colors[box[k].color], box[k].index);
                    if(k != (i-1))
                        printf(", ");
                }
                printf("\n");
            }
            pthread_mutex_unlock(&mutex);
            pthread_exit(0);
        }
        
        /* packer extract products one at a time from the assembler line and put in the box */
        
        box[i].color = buffer[out].color;
        box[i].index = buffer[out].index;
        
        /* packer prints out all its contents if the box is full and go to extract products again */
        
        if(i==n-1) {
            
            printf("[Packer %04ld]: I have a box of products containing: ",(long)pthread_self()%10000);
            
            for(j=0;j<n;j++) {
                printf("%s%d",colors[box[j].color], box[j].index);
                if(j != (n-1))
                    printf(", ");
            }
            
            /* reset the box the retrive products again */
            i=-1;
            printf("\n");
        }
        
        out = (out+1)%l;
        count--;

        pthread_cond_signal(&cond_a);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(0);
}
