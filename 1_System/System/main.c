//
//  main.c
//  1_System
//
//  Created by Valentin Rep on 14/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char * argv[]) {
    
    int ifd, ofd;
    unsigned char buffer[1024];
    
    if ( argc != 3 ) {
        write(STDERR_FILENO, "Invalid number of arguments\n", 29);
        write(STDOUT_FILENO, "Usage: program input_file output_file\n", 39);
        return 1;
    }
    
    if ( (ifd = open(argv[1], O_RDONLY)) == -1 ) {
        write(STDERR_FILENO, "Error opening output file\n", 27);
        return 1;
    }
    
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if ( (ofd = open(argv[2], flags, perms)) == -1 ) {
        write(STDERR_FILENO, "Error opening output file\n", 27);
        return 1;
    }
    
    while ( read(ifd, &buffer, sizeof(buffer)) > 0 ) {
        write(ofd, &buffer, sizeof(buffer));
    }
    
    write(STDOUT_FILENO, "System: File copy completed\n", 29);
    
    close(ifd);
    close(ofd);
    
    return 0;
}
