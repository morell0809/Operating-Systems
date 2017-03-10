//
// Created by morell on 08.03.17.
//

#ifndef STRINGS_TEST_H
#define STRINGS_TEST_H


#include "stdio.h"
#include <stdlib.h>

void test_func(int argc, char** argv) {

    int val;
    int count = 0;

    printf("Hello\n");
    for( int i = 0; i < argc; i++ ){
        printf("%s\n",argv[i]);
    }
    printf("\n");
    while( (val = getopt(argc,argv,"vi")) != -1 ){
        switch (val){
            case 'i':
                printf("i param\n");
                break;
            case 'v':
                printf("v param\n");
                break;
            case '?':
                printf("error statement");
                break;
            default:
                printf("default");
        }
        printf("optind: %d\n", optind);
        count++;
        fflush(stdout);
    }
    printf("END optind: %d\n", optind);

}

#endif //STRINGS_TEST_H
