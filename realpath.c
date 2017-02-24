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
// Верх. путь
void pwd(ino_t, string* );
void inum_to_name(ino_t, string* , int);
//
// Пропускает все delim version = 1, выравнить на последний version = 0
int skip_delim( string* , int , char, int version );
// Выравнивает на след. delim
int next_delim( string* res, int pos, char delim );
// Нижн.путь
void pwddown( string* res, string* parse_path, int pos );

int main()
{
    string* input = (string*) malloc(sizeof(string));
    read_str(input);
    if(input->len){
        // След.позиция в input для чтения sub_path
        int read_pos = 0;
        // Путь до пробела
        string* sub_path = (string*) malloc(sizeof(string));
        while( input->str[read_pos] ){
            int left = read_pos;
            int right = next_delim(input,read_pos,' ');
            read_pos = skip_delim(input,right,' ', 1);
            substr(input,sub_path,left,right - 1);
            // Идем не с корня
            if(sub_path->str[0] != '/' ){
                string* realup = (string*) malloc(sizeof(string));
                string* realdown = (string*) malloc(sizeof(string));
                front_add(&sub_path,"/");
                // Верхний путь
                /*pwd(get_inode("."),realup);
                reverse(realup);
                print_dir_char(".");
                print_dir(realup);
                chdir(realup->str);
                print_dir_char(".");*/
                // Нижний путь.
                append(realdown, '/' );
                pwddown(realdown,sub_path,0);
                cat_str(realup,realdown);
                print(realup);
                del_str(realup);
                del_str(realdown);
                del_str(sub_path);
            } else{// С корня
                string* realdown = (string*) malloc(sizeof(string));
                pwddown(realdown,sub_path,0);
                print(realdown);
            }
        }
        del_str(sub_path);
        /*int dot_count = 0;// # точек
        int i = 0;
        while(input->str[i] !='/'){
            if( input->str[i] == '.'){
                dot_count++;
            }
        }
        if( dot_count == 1 ){

        } else if( dot_count == 2){

        } else if (dot_count == 0 && input->str[0] == '/' ){
            //root
        } else{
            // Копирование имени проверяемой директории
            string* check_file = (string*) malloc(sizeof(string));
            substr(input,check_file,0,i-1);
            // Проверка совпадения имен директорий
            int cur_inode = get_inode(".");
            chdir("..");
            string* its_name = (string*) malloc(sizeof(string));

            inum_to_name(cur_inode,its_name,BUFSIZ);
            // Директория сущ.
            if( !strcmp(its_name,check_file)){
                // Вверх
                pwd(get_inode("."),realup);
                // Вниз
                pwddown()
            } else{// Не сущ.
                // После '/' ничего нет
                if( i == input->len - 1 ){
                    chdir(its_name->str);
                    // Вверх
                    pwd(get_inode("."),realup);
                    print(realup);
                } else{// Есть
                    perror("");
                    exit(1);
                }
            }
        }*/

    } else{
        printf("%s\n","realpath: missing operand");
    }
    del_str(input);
    return 0;
}

void is_null( string* res, char* name_func)
{
    if(!res) {
        perror("res is NULL in ");
        perror(name_func);
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
        if(!chdir("..") ){// Вверх по катоологу
            inum_to_name(this_inod,its_name,BUFSIZ);// Получить имя католога
            reverse(its_name);
            cat_str(res,its_name);
            append(res,'/');
            my_inode = get_inode(".");
            pwd(my_inode,res);
        } else{
            fprintf(stderr,"Bad up");
        }
    }
    free(its_name);
}

// str[pos] == delim => свдинуть pos до последнего delim
int skip_delim( string* res, int pos, char delim, int version )
{
    if( res->str[pos] == delim ){
        while(res->str[pos] == delim ){
            pos++;
        }
        if( !version ){
            pos--;
        }
    }
    return pos;
}

int next_delim( string* res, int pos, char delim )
{
    if(res->str[pos] == delim){
        pos++;
    }
    while(res->str[pos] != delim && res->str[pos] != '\0' ){
        pos++;
    }
    return pos;
}

int is_dot( string* res )
{
    return !strcmp(res->str,".") || !strcmp(res->str,"..");
}

void pwddown( string* res, string* parse_path, int pos )
{
    is_null(res,"pwddown");
    is_null(parse_path,"pwddown");
    if(parse_path->str[pos] != '/'){
        perror("Illegal path");
        exit(1);
    }

    string* its_name = (string*) malloc(sizeof(string));
    char_init(its_name,"");
    // Если мы не в корне
    while( parse_path->str[pos] != '\0' ) {
        int left = pos + 1;
        // К следующему /
        int right = next_delim(parse_path,pos,'/');
        pos = right;
        // Скип '////////' до последнего
        pos = skip_delim(parse_path,pos,'/',0);
        substr(parse_path,its_name,left,right - 1);
        struct stat stat_info;
        stat(its_name->str, &stat_info);
        // Файл найден
        if(!stat(its_name->str, &stat_info)){
            // Точки => переход
            if( is_dot(its_name) ){
                print_dir(its_name);
                chdir(its_name->str);
                print_dir(its_name);
                del_str(its_name);
                pwddown(res, parse_path, pos);
                return;
            } else if(S_ISDIR(stat_info.st_mode)){// Директория. Добавить имя => парсить
                cat_str(res,its_name);
                chdir(its_name->str);
                del_str(its_name);
                pwddown(res, parse_path, pos);
                return;
            } else if(S_ISREG(stat_info.st_mode)){
                if( parse_path->str[pos] != '\0' ){// Обычный файл и не допарсили
                    perror("No such file or directory");
                    del_str(its_name);
                    exit(1);
                } else{
                    cat_str(res,its_name);
                    del_str(its_name);
                    return;
                }
            } else if(S_ISLNK(stat_info.st_mode)){// Симв. ссылка => парсить
                chdir(its_name->str);
                del_str(its_name);
                pwddown(res, parse_path, pos);
                return;
            } else{
                perror("Illegal file");
                del_str(its_name);
                exit(1);
            }
        } else if( parse_path->str[pos] != '\0' ){// Файла нет и не допарсили
            perror("No such file or directory");
            del_str(its_name);
            exit(1);
        } else{
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
            char_init(its_name,direntp->d_name);
            closedir(dir_ptr);
            return;
        }
    }

    fprintf(stderr,"Error find inode");
    exit(1);
}

/*// Найти в катологе файл с данным inod(inode_to_find) и скопировать его в name_buf
ino_t find_inum(string* its_name )
{
    is_null(its_name,"find_inum");

    DIR* dir_ptr = 0;
    struct dirent* direntp = 0;
    dir_ptr = opendir(".");
    if( !dir_ptr ) {
        perror("Bad open '.'");
        exit(1);
    }
    // Ищем католог с заданным its_name
    *//*while((direntp = readdir(dir_ptr)) != 0 ) {
        if( !strcmp(direntp->d_name,its_name->str) ) {
            closedir(dir_ptr);
            return direntp->d_ino;
        }
    }*//*


    fprintf(stderr,"Error find inode");
    exit(
}*/
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
