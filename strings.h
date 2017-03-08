//
// Created by morell on 19.02.17.
//

#ifndef STRINGS_STRINGS_H
#define STRINGS_STRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

//Усл.
// cap на +1 больше для '\0' в конце
// левый аргумент в функции может быть NULL

const int DEF_MAXSIZE = BUFSIZ;

typedef struct Str {
    //Зам. всегда динамич. выделение памяти
    char *str;
    int len;
    int cap;
    // Индекс начиная с которго можно писать.
    // Зам.Все что правее, несуществует. pos = -1 - буфер заполнен
    int pos;
} string;

// Зам. Проверка пустоты по length
int empty_len(const string *val) {
    if (val->len == 0) { return 1; }
    else { return 0; }
}

// Зам. Проверка пустоты по capacity
int empty_cap(const string *val) {
    if (val->cap == 0) { return 1; }
    else { return 0; }
}

// Создать строку res
// Зам. val - пустая => res - пустая с capacity по умолчанию
// res - приходит NULL
// val - не пуст => откопировать val в res; capacity - забито
// res - всегда динам.выделение памяти, val - стек || динам.

// Инициализация res строкой val type:char*
void char_init(string *res, char *val, int capacity) {
    if (!val || !res) {
        perror("val is NULL");
        exit(1);
    }

    int length = strlen(val);
    res->str = (char *) malloc((length + capacity + 1) * sizeof(char));
    res->len = length;
    res->cap = res->len + capacity;
    res->pos = res->len;
    for (int i = 0; i <= res->len; i++) {

        res->str[i] = val[i];

    }

}

// Инициализация res через val
void string_init(string *res, const string *val) {
    if (!val || !res || !val->str) {

        perror("val is NULL");
        exit(1);

    }

    const int add_space = 10;
    res->str = (char *) malloc((val->len + add_space + 1) * sizeof(char));
    res->len = val->len;
    res->cap = res->len + add_space;
    res->pos = res->len;
    for (int i = 0; i <= val->len; i++) {

        res->str[i] = val->str[i];

    }

}

// Копирует в dest строку from
// Зам: dest.cap >= from.cap
// dest - всегда динам.выделение памяти, from - стек || динам.
void copy_str(string *dest, const string *from) {
    if (!dest || !from || !from->str) {

        perror("val is NULL");
        exit(1);

    }
    if (empty_cap(from)) {

        free(dest->str);
        dest->str = 0;
        dest->len = 0;
        dest->cap = 0;
        dest->pos = 0;

    } else if (empty_cap(dest)) {

        string_init(dest, from);

    } else {
        if (dest->cap <= from->len) {

            free(dest->str);
            dest->str = (char *) malloc((2 * from->cap + 1) * sizeof(char));
            dest->len = from->len;
            dest->cap = 2 * from->cap;
            dest->pos = from->len;

        } else {

            dest->len = from->len;
            dest->pos = from->len;

        }
        for (int i = 0; i <= dest->len; i++) {
            dest->str[i] = from->str[i];
        }
    }
}

// Конкатенация res и val
// Зам. res - всегда динам.выделение памяти, val - стек || динам.
void cat_str(string *res, const string *const val) {
    if (!val || !res || !val->str) {

        perror("val is NULL");
        exit(1);

    }
    if (!empty_len(val)) {
        // Свободно место для записи в res
        int freespace = res->cap - res->len;
        if (freespace <= val->len) {

            // Новое cap для res
            int newcap = 2 * (res->len + val->len);
            // tmp - временная строка для копирования
            char *tmp = (char *) malloc((newcap + 1) * sizeof(char));
            // Копируем в tmp старый res
            if (res->len) {

                for (int i = 0; i <= res->len; i++) {
                    tmp[i] = res->str[i];
                }

            }
            // Конкатенируем tmp и res
            int curpos = res->len;
            for (int i = 0; i <= val->len; i++) {

                tmp[curpos + i] = val->str[i];

            }
            // Перевязываем res на tmp
            free(res->str);
            res->str = tmp;
            tmp = 0;
            res->len = val->len + res->len;
            res->cap = newcap;
            res->pos = res->len;

        } else {
            // Пространства  в res хватает => конкатенируем с позиции res_pos
            for (int i = 0; i <= val->len; i++) {

                res->str[i + res->pos] = val->str[i];

            }
            res->len = val->len + res->len;
            res->pos += val->len;
        }
    }
}

// Очистить поля
void clear_str(string *val) {
    if (val->str) {

        free(val->str);
        val->str = 0;
        val->len = 0;
        val->cap = 0;
        val->pos = -1;

    }
}

// Удалить структуру
void del_str(string *val) {
    if (val) {

        clear_str(val);
        free(val);

    }
}

// Увеличить размер val на add символов
string *resize(string *val, const int add) {
    if (!val || !val->str) {

        perror("val is NULL");
        exit(1);

    }
    string *tmp = (string *) malloc(sizeof(string));
    tmp->str = malloc((val->cap + add + 1) * sizeof(char));
    copy_str(tmp, val);
    del_str(val);

    return tmp;
}

void append(string *val, char c) {
    if (!val) {

        perror("val is NULL");
        exit(1);

    }
    if (!val->str) {

        char_init(val, "", BUFSIZ);

    }
    if (val->cap == val->len) {

        val = resize(val, DEF_MAXSIZE);

    }
    val->str[val->pos] = c;
    val->pos++;
    val->len++;
    val->str[val->len] = '\0';
}

void print(const string *val) {
    printf("%s\n", val->str);
}

// Читает с stdin строк длины ull в res
void read_str(string *res) {
    if (!res) {

        perror("res is NULL in read_str");
        exit(1);

    }
    res->str = 0;
    size_t mem_size = 0;
    ssize_t read = getline(&res->str, &mem_size, stdin);
    res->str[read - 1] = '\0';
    res->len = read - 1;
    res->cap = read;
    res->pos = read - 1;
}

int read_one_str(string *res, FILE **from) {

    if (!res) {

        perror("res is NULL in read_str");
        exit(1);

    }

    res->str = 0;
    size_t mem_size = BUFSIZ;
    ssize_t read = 0;
    if (from[0]) {
        read = getline(&res->str, &mem_size, from[0]);
    } else {
        read = getline(&res->str, &mem_size, stdin);
    }

    if (read == -1) {

        return -1;

    } else {
        res->str[read - 1] = '\0';
        res->len = read - 1;
        res->cap = read;
        res->pos = read - 1;
        printf("%s", res->str);
        char c = 'a';
        for (int i = 0; i < res->len; i++) {
            c = res->str[i];
        }
        if (strlen(res->str) != res->len) {
            perror("read_one_str: Bad len %s");
            exit(1);
        }

        return read;
    }

}

void reverse(string *res) {
    if (!res) {

        perror("val is NULL");
        exit(1);

    } else {
        for (int i = 0; i < res->len / 2; i++) {

            char c = res->str[i];
            res->str[i] = res->str[res->len - 1 - i];
            res->str[res->len - 1 - i] = c;

        }
    }
}

int cmp_str(string *left, string *right) {
    if (!left || !right) {

        perror("left is  NULL in equal_str");
        exit(1);

    }
    return strcmp(left->str, right->str);
}

// Копирует from[left,right] в dest
void substr(string *from, string *dest, int left, int right) {
    if (!from || !dest) {

        perror("from || dest is NULL in substr");
        exit(1);

    }
    if (!dest->str || dest->cap < right - left + 1) {
        if (dest->str) {

            free(dest->str);

        }
        dest->str = (char *) malloc(right - left + 2);
        dest->len = right - left + 1;
        dest->cap = right - left + 1;
        dest->pos = -1;

    } else {

        dest->len = right - left + 1;
        dest->pos = right - left + 1;

    }
    for (int i = left; i <= right; i++) {

        dest->str[i - left] = from->str[i];

    }
    dest->str[right - left + 1] = '\0';
}

void front_add(string **val, char *front) {
    if (!val || !*val || !(*val)->str) {

        perror("val is NULL");
        exit(1);

    }
    string *tmp = (string *) malloc(sizeof(string));
    int front_len = strlen(front);
    if ((*val)->cap - (*val)->len < DEF_MAXSIZE * 0.1) {

        tmp->str = (char *) malloc(((*val)->len + front_len + DEF_MAXSIZE + 1) * sizeof(char));
        tmp->cap = (*val)->len + front_len + DEF_MAXSIZE;

    } else {

        tmp->str = (char *) malloc(((*val)->len + front_len + 1) * sizeof(char));
        tmp->cap = (*val)->len + front_len;

    }
    for (int i = 0; i < front_len + (*val)->len; i++) {
        if (i < front_len) {

            tmp->str[i] = front[i];

        } else {

            tmp->str[i] = (*val)->str[i - front_len];

        }
    }
    tmp->len = front_len + (*val)->len;
    tmp->pos = front_len + (*val)->len;
    del_str(*val);
    *val = tmp;
}


#endif //STRINGS_STRINGS_H
