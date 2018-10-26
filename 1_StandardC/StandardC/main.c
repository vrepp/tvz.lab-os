//
//  main.c
//  1_StandardC
//
//  Created by Valentin Rep on 14/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    FILE *ifp, *ofp;
    unsigned char buffer[1024];
    
    if ( argc != 3 ) {
        printf("Invalid number of arguments\n");
        printf("Usage: program input_file output_file\n");
        return 1;
    }
    
    if ( (ifp = fopen(argv[1], "r")) == NULL ) {
        printf("Error opening input file");
        return 1;
    }
    
    if ( (ofp = fopen(argv[2], "w")) == NULL ) {
        printf("Error opening output file");
        return 1;
    }
    
    while ( fread(&buffer, sizeof(buffer), 1, ifp) > 0 ) {
        fwrite(&buffer, sizeof(buffer), 1, ofp);
    }
    
    printf("Standard C: File copy completed\n");
    
    fclose(ifp);
    fclose(ofp);
    
    return 0;
}

