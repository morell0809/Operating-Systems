//
// Created by morell on 12.03.17.
//

#include <stdio.h>
#include <libgen.h>
#include "strings.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ino_t get_inode( char* );
// Верх. путь
void pwd(ino_t, string* );
void inum_to_name(ino_t, string* , int);
// Пропускает все delim version = 1, выравнить на последний version = 0
int skip_delim( string* , int , char, int version );
// Выравнивает на след. delim
int next_delim( string* res, int pos, char delim );
// Нижн.путь
void pwddown( string* res, string* parse_path, int pos );

int main( int argc, int* argv[] )
{
    string* input = (string*) malloc(sizeof(string));
    read_str(input);

    if(input->len) {
        // След.позиция в input для чтения sub_path
        int read_pos = 0;
        // Путь до пробела
        string* sub_path = (string*) malloc(sizeof(string));
        while( input->str[read_pos] ) {

            int left = read_pos;
            int right = next_delim(input,read_pos,' ');
            read_pos = skip_delim(input,right,' ', 1);
            substr(input,sub_path,left,right - 1);
            // Идем не с корня
            if(sub_path->str[0] != '/' ) {

                string* realup = (string*) malloc(sizeof(string));
                string* realdown = (string*) malloc(sizeof(string));
                front_add(&sub_path,"/");
                // Верхний путь
                // print_dir_char(".");
                pwd(get_inode("."),realup);
                reverse(realup);
                // print_dir_char(".");
                // // print_dir(realup);
                //chdir("/home/morell/.CLion2016.2/system/cmake/generated/Str-be78cd4a/be78cd4a/Debug/Strings");
                //print_dir_char("/home/morell/.CLion2016.2/system/cmake/generated/Str-be78cd4a/be78cd4a/Debug/Strings");
                // Нижний путь.
                append(realdown, '/' );
                cat_str(realup,sub_path);
                pwddown(realdown,realup,0);
                cat_str(realup,realdown);
                print(realdown);
                del_str(realup);
                del_str(realdown);;

            } else {// С корня
                chdir('/');
                string* realdown = (string*) malloc(sizeof(string));

                pwddown(realdown,sub_path,0);
                print(realdown);
                del_str(realdown);

            }
        }
        del_str(sub_path);

    } else {

        printf("%s\n","realpath: missing operand");

    }
    del_str(input);
    return 0;
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

int skip_delim( string* res, int pos, char delim, int version )
{
    if( res->str[pos] == delim ) {
        while(res->str[pos] == delim ) {

            pos++;

        }
        if( !version ) {

            pos--;

        }
    }
    return pos;
}

int next_delim( string* res, int pos, char delim )
{
    if(res->str[pos] == delim) {

        pos++;

    }
    while(res->str[pos] != delim && res->str[pos] != '\0' ) {

        pos++;

    }
    return pos;
}

int is_dot( string* res )
{
    return !strcmp(res->str,".") || !strcmp(res->str,"..");
}

void pwddown( string* res, string* parse_path, int left )
{
    is_null(res,"pwddown");
    is_null(parse_path,"pwddown");

    // Пропустить '////////' без выравнивания
    left = skip_delim(parse_path,left,'/',1);
    if( parse_path->str[left] == '\0' ) {

        return;

    }

    string* its_name = (string*) malloc(sizeof(string));
    char_init(its_name,"",BUFSIZ);
    struct stat stat_info;
    // К следующему /
    int right = next_delim(parse_path,left,'/');
    substr(parse_path,its_name,left,right - 1);
    print_dir();
    // Файл найден
    if(!lstat(its_name->str, &stat_info)) {

        printf("%o\n",S_IFLNK);
        int a = stat_info.st_mode & 0170000;
        printf("%o\n",stat_info.st_mode & 0170000);
        if( is_dot(its_name) ) {

            chdir(its_name->str);
            del_str(its_name);
            pwddown(res, parse_path, right);

        } else if(S_ISLNK(stat_info.st_mode)) {// Симв. ссылка => парсить

            // print_dir(its_name);
            chdir(its_name->str);
            //print_dir(its_name);
            del_str(its_name);
            pwddown(res, parse_path, right);

        } else if(S_ISDIR(stat_info.st_mode)) {// Директория. Добавить имя => парсить

            // print_dir(its_name);
            cat_str(res,its_name);
            append(res,'/');
            chdir(its_name->str);
            //       // print_dir(its_name);
            del_str(its_name);
            pwddown(res, parse_path, right);

        } else if(S_ISREG(stat_info.st_mode)) {
            if( parse_path->str[right] != '\0' ) {// Обычный файл и не допарсили

                perror("No such file or directory");
                del_str(its_name);
                exit(1);

            } else {

                cat_str(res,its_name);
                del_str(its_name);

            }
        } else {

            perror("Illegal file");
            del_str(its_name);
            exit(1);

        }
        // Точки => переход
    } else if( parse_path->str[right] != '\0' ) {// Файла нет и не допарсили

        perror("No such file or directory");
        del_str(its_name);
        exit(1);

    } else {

        // / Файла нет и допарсили
        cat_str(res,its_name);
        append(res,'/');
        del_str(its_name);

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

            char_init(its_name,direntp->d_name,BUFSIZ);
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