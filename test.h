//
// Created by morell on 08.03.17.
//

#ifndef STRINGS_TEST_H
#define STRINGS_TEST_H


#include "stdio.h"
#include <stdlib.h>
#include <sys/stat.h>

void test_func(int argc, char** argv) {

    struct stat stat_info;
    //print_dir_char(".");
    // Файл найден
    int a  = lstat("/home/morell/Untitled 1.odt",&stat_info);
    if( a == -1 ){
        printf("bad");
        exit(1);
    } else{

        long int a = stat_info.st_mode & 0170000;
       if (S_ISLNK(stat_info.st_mode)) {// Симв. ссылка. Не добавлять => парсить
           printf("mode: %a", a);
           printf("lnk");

        } else if (S_ISDIR(stat_info.st_mode)) {// Директория. Добавить имя => парсить
            printf("dir");

        } else if (S_ISREG(stat_info.st_mode)) {
            printf("file");
        }
    }
}

#endif //STRINGS_TEST_H
