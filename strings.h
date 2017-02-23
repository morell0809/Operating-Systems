//
// Created by morell on 19.02.17.
//

#ifndef STRINGS_STRINGS_H
#define STRINGS_STRINGS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Усл.
// cap на +1 больше для '\0' в конце
// Левый может быть NULL
const int DEF_MAXSIZE = BUFSIZ;

typedef struct Str
{
    //Зам. всегда динамич. выделение памяти
    char* str;
    int len;
    int cap;
    // Индекс начиная с которго можно писать.
    // Зам.Все что правее, несуществует. pos = -1 - буфер заполнен
    int pos;
}string;

// Зам. Проверка пустоты по length
int empty_len( string* str_val )
{
    if( str_val->len == 0 ){ return 1; }
    else{ return 0; }
}

// Зам. Проверка пустоты по capacity
int empty_cap( string* str_val )
{
    if( str_val->cap == 0 ){ return 1; }
    else{ return 0; }
}

// Создать строку str_res
// Зам. str_val - пустая => str_res - пустая с capacity по умолчанию
// str_res - приходит NULL
// str_val - не пуст => откопировать str_val в str_res; capacity - забито
// str_res - всегда динам.выделение памяти, str_val - стек || динам.

// Инициализация res строкой val type:char*
void char_init( string* str_res, char* str_val )
{
    if( !str_val || !str_res )
    {
        perror("val is NULL");
        exit(1);
    }

    int length = strlen(str_val);
    const int add_space = 10;
    str_res->str = (char*) malloc( ( length + add_space + 1 ) * sizeof(char));
    str_res->len = length;
    str_res->cap = str_res->len + add_space;
    str_res->pos = str_res->len;
    for( int i = 0; i <= str_res->len; i++ ) {
        str_res->str[i] = str_val[i];
    }

}

// Инициализация res строкой val type:string
void string_init( string* str_res, const string* str_val )
{
    if( !str_val || !str_res || !str_val->str )
    {
        perror("val is NULL");
        exit(1);
    }

    const int add_space = 10;
    str_res->str = (char*) malloc((str_val->len + add_space + 1)*sizeof(char));
    str_res->len = str_val->len;
    str_res->cap = str_res->len + add_space;
    str_res->pos = str_res->len;
    for( int i = 0; i <= str_val->len; i++ ){
        str_res->str[i] = str_val->str[i];
    }

}

// Копирует в dest строку from
// Зам: dest.cap >= from.cap
// dest - всегда динам.выделение памяти, from - стек || динам.
void copy_str( string* dest, const string* from )
{
    if( !dest || !from || !from->str )
    {
        perror("val is NULL");
        exit(1);
    }
    if( empty_cap(from) ) {
        free(dest->str);
        dest->str = 0;
        dest->len = 0;
        dest->cap = 0;
        dest->pos = 0;
    } else if( empty_cap(dest) ){
        string_init(dest,from);
    } else {
        if( dest->cap <= from->len ){
            free(dest->str);
            dest->str = (char*) malloc((2 * from->cap + 1)*sizeof(char));
            dest->len = from->len;
            dest->cap = 2*from->cap;
            dest->pos = from->len;
        } else {
            dest->len = from->len;
            dest->pos = from->len;
        }
        for( int i = 0; i <= dest->len; i++ )
        {
            dest->str[i] = from->str[i];
        }
    }
}

// str_res - всегда динам.выделение памяти, str_val - стек || динам.
void cat_str( string* str_res, const string* str_val )
{
    if( !str_val || !str_res || !str_val->str )
    {
        perror("val is NULL");
        exit(1);
    }
    if( !empty_len(str_val) ) {
        // Свободно место для записи в str_res
        int freespace = str_res->cap - str_res->len;
        if( freespace <= str_val->len ){
            // Новое cap для str_res
            int newcap = 2 * ( str_res->len + str_val->len);
            // tmp - временная строка для копирования
            char* tmp = (char*) malloc( ( newcap + 1) * sizeof(char) );
            // Копируем в tmp старый str_res
            if( str_res->len ){
                for( int i = 0; i <= str_res->len; i++ ){
                    tmp[i] = str_res->str[i];
                }
            }
            // Конкатенируем tmp и str_res
            int curpos = str_res->len;
            for( int i = 0; i <= str_val->len; i++ ){
                tmp[curpos+i] = str_val->str[i];
            }
            // Перевязываем str_res на tmp
            free(str_res->str);
            str_res->str = tmp;
            tmp = 0;
            str_res->len = str_val->len + str_res->len;
            str_res->cap = newcap;
            str_res->pos = str_res->len;
        } else {
            // Пространства  в str_res хватает => конкатенируем с позиции str_res_pos
            for( int i = 0; i <= str_val->len; i++ ) {
                str_res->str[i+str_res->pos] = str_val->str[i];
            }
            str_res->len = str_val->len + str_res->len;
            str_res->pos += str_val->len;
        }
    }
}

// Очистить поля
void clear_str( string* str_val )
{
    if( str_val->str ){
        free(str_val->str);
        str_val->str = 0;
        str_val->len = 0;
        str_val->cap = 0;
        str_val->pos = -1;
    }
}

// Удалить структуру
void del_str( string* str_val )
{
    if(str_val){
        clear_str(str_val);
        free(str_val);
    }
}

// Увеличить размер str_val на add символов
string* resize( string* str_val, const int add )
{
    if( !str_val || !str_val->str ){
        perror("val is NULL");
        exit(1);
    }
    string* tmp = (string*) malloc(sizeof(string));
    tmp->str = malloc((str_val->cap + add + 1) * sizeof(char));
    copy_str(tmp, str_val);
    del_str(str_val);

    return tmp;
}

void append( string* str_val, char c )
{
    if( !str_val || !str_val->str ){
        perror("val is NULL");
        exit(1);
    }
    if( str_val->cap == str_val->len ) {
        str_val = resize(str_val, DEF_MAXSIZE);
    }
    str_val->str[str_val->pos] = c;
    str_val->pos++;
    str_val->len++;
    str_val->str[str_val->len] = '\0';
}

void print( const string* str_val )
{
    printf("%s\n", str_val->str );
}

// Читает с stdin строк длины ull в res
void read_str( string* res )
{

}

void reverse( string* res )
{
    if( !res  ){
        perror("val is NULL");
        exit(1);
    } else{
       for( int i = 0; i < res->len / 2; i++ ){
           char c = res->str[i];
           res->str[i] = res->str[res->len - 1 - i];
           res->str[res->len - 1 - i] = c;
       }
   }
}
#endif //STRINGS_STRINGS_H
