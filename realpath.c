//
// Created by morell on 19.02.17.
//
#include <stdio.h>
#include <libgen.h>
#include "strings.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ino_t get_inode( char* );
void print_path_to(ino_t, string* );
void inum_to_name(ino_t, string* , int);

int main()
{
    string* res = (string*) malloc( sizeof(string));
    char_init(res,"");
    print_path_to(get_inode("."),res);
    reverse(res);
    print(res);
    free(res);
    return 0;
}

void print_path_to( ino_t this_inod, string* res )
{
    ino_t my_inode;
    string* its_name = (string*) malloc(sizeof(string));
    // Если мы не в корне
    int check = get_inode("..");
    if( check != this_inod ) {
        if(!chdir("..") ){// Вверх по катоологу
            inum_to_name(this_inod,its_name,BUFSIZ);// Получить имя католога
            reverse(its_name);
            cat_str(res,its_name);
            append(res,'/');
            my_inode = get_inode(".");
            print_path_to(my_inode,res);
        } else{
            fprintf(stderr,"Bad up");
        }
    }
    free(its_name);
}

// Найти в катологе файл с данным inod(inode_to_find) и скопировать его в name_buf
void inum_to_name(ino_t inode_to_find, string* its_name, int size )
{
    DIR* dir_ptr = 0;
    struct dirent* direntp = 0;

    dir_ptr = opendir(".");
    if( !dir_ptr ){
        perror("Bad open '.'");
        exit(1);
    }
    if(!its_name) {
        perror("its_name is NULL");
        exit(1);
    }

    dir_ptr = opendir(".");
    // Ищем католог для файла с заданным inode_to_find
    while((direntp = readdir(dir_ptr)) != 0 ) {
        if( direntp->d_ino == inode_to_find ) {
            char_init(its_name,direntp->d_name);
            closedir(dir_ptr);
            return;
        }
    }
    dir_ptr = opendir(".");
    // Ищем католог для файла с заданным inode_to_find
    while((direntp = readdir(dir_ptr)) != 0 ) {
        printf("%s",direntp->d_name);

    }
    fprintf(stderr,"Error find inode");
    exit(1);
}

ino_t get_inode(char* file_name)
{
    struct stat stat_info;
    if(!stat(file_name, &stat_info)){
        return stat_info.st_ino;
    } else {
        perror("Error stat");
        exit(1);
    }
}
