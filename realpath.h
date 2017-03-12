//
// Created by morell on 11.03.17.
//

#ifndef STRINGS_REALPATH_H
#define STRINGS_REALPATH_H

#include "strings.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void remove_back(string* res) {
    int count_slash = 0;
    for (int i = res->len - 1; i >= 0; i--) {
        if (res->str[i] == '/') {
            count_slash++;
            if (count_slash == 2) {
                res->str[i + 1] = '\0';
                res->len = i + 1;
                res->pos = i + 1;
                break;
            }
        }
    }
}


int skip_delim(string* res, int pos, char delim, int version) {
    if (res->str[pos] == delim) {
        while (res->str[pos] == delim) {

            pos++;

        }
        if (!version) {

            pos--;

        }
    }
    return pos;
}

int next_delim(string* res, int pos, char delim) {
    if (res->str[pos] == delim) {

        pos++;

    }
    while (res->str[pos] != delim && res->str[pos] != '\0') {

        pos++;

    }
    return pos;
}

int is_dot(string* res) {
    return !strcmp(res->str, ".") || !strcmp(res->str, "..");
}


int count = 0;
// Формат parse_path = "/..../". Формат res = "/..../".
void pwddown(string* res, string* parse_path, int left) {
    is_null(res, "pwddown");
    is_null(parse_path, "pwddown");

    // Пропустить '////////' без выравнивания
    left = skip_delim(parse_path, left, '/', 1);
    if (parse_path->str[left] == '\0') {

        return;

    }

    string* its_name = (string*) malloc(sizeof(string));
    char_init(its_name, "", BUFSIZ);
    struct stat stat_info;
    // К следующему / с выравниванием.
    int right = next_delim(parse_path, left, '/');
    substr(parse_path, its_name, left, right - 1);

    count++;
    // Откат назад
    if (!cmp_char(its_name, "..")) {

        remove_back(res);
        del_str(its_name);
        chdir("..");
        pwddown(res, parse_path, right);

    } else if (!cmp_char(its_name, ".")) {
        pwddown(res, parse_path, right);
    }
    else {
        // Файл найден
        if(!strcmp(its_name->str,"morell")){
            print_dir();
        }
        if (!lstat(its_name->str, &stat_info)) {
            if (S_ISLNK(stat_info.st_mode)) {// Симв. ссылка. Не добавлять => парсить

                chdir(its_name->str);
                pwddown(res, parse_path, right);
                del_str(its_name);

            } else if (S_ISDIR(stat_info.st_mode)) {// Директория. Добавить имя => парсить
                cat_str(res,its_name);
                if( parse_path->str[right] != '\0'){
                    append(res, '/');
                }
                chdir(its_name->str);
                pwddown(res, parse_path, right);
                del_str(its_name);

            } else if (S_ISREG(stat_info.st_mode)) {
                if (parse_path->str[right] != '\0') {// Обычный файл и не допарсили

                    perror("No such file or directory");
                    del_str(its_name);
                    exit(1);

                } else {

                    cat_str(res, its_name);
                    del_str(its_name);

                }
            } else {
                perror("Illegal file");
                del_str(its_name);
                exit(1);

            }
        } else if (parse_path->str[right] != '\0') {// Файла нет и не допарсили
            perror("No such file or directory");
            del_str(its_name);
            exit(1);

        } else {

            // / Файла нет и допарсили
            cat_str(res,its_name);
            del_str(its_name);

        }
    }

}

// Найти в катологе файл с данным inod(inode_to_find) и скопировать его в name_buf
void inum_to_name(ino_t inode_to_find, string* its_name, int size )
{
    if(!its_name) {

        perror("its_name is NULL");
        exit(1);

    }

    DIR* dir_ptr = 0;
    struct dirent* direntp = 0;

    dir_ptr = opendir(".");
    if( !dir_ptr ) {

        perror("Bad open '.'");
        exit(1);

    }
    // Ищем католог для файла с заданным inode_to_find
    while((direntp = readdir(dir_ptr)) != 0 ) {
        if( direntp->d_ino == inode_to_find ) {

            char_init(its_name,direntp->d_name,strlen(direntp->d_name) + 1);
            closedir(dir_ptr);
            return;

        }
    }

    fprintf(stderr,"Error find inode");
    exit(1);
}

ino_t get_inode(char* file_name)
{
    struct stat stat_info;
    if(!stat(file_name, &stat_info)) {

        return stat_info.st_ino;

    } else {

        perror("Error stat");
        exit(1);

    }
}

void pwd( ino_t this_inod, string* res )
{
    if(!res) {

        perror("res is NULL in print_path");
        exit(1);

    }

    ino_t my_inode;
    string* its_name = (string*) malloc(sizeof(string));
    // Если мы не в корне
    int check = get_inode("..");
    if( check != this_inod ) {

        if(!chdir("..") ) {// Вверх по катоологу

            inum_to_name(this_inod,its_name,BUFSIZ);// Получить имя католога
            reverse(its_name);
            cat_str(res,its_name);
            append(res,'/');
            my_inode = get_inode(".");
            pwd(my_inode,res);

        } else {

            fprintf(stderr,"Bad up");

        }
    } 
    free(its_name);
}


#endif //STRINGS_REALPATH_H
