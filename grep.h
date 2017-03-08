//
// Created by morell on 08.03.17.
//

#ifndef STRINGS_GREP_H
#define STRINGS_GREP_H

#include <ctype.h>

// Префикс функция. argv[0] == i. argv[1] == v.
int* pref_func(const string* const val, int* argv) {
    static int call = 0;
    int iter = 0;

    int* pref = malloc(val->len * sizeof(int));
    int pos = 0;

    pref[0] = 0;
    for (int i = 1; i < val->len; i++) {
        pos = pref[i - 1];
        int v1 = val->str[i] != val->str[pos];
        int v2 = tolower(val->str[i]) != tolower(val->str[pos]);
        while (pos > 0 && (!argv[0] && v1 || argv[0] && v2)) {
            pos = pref[pos - 1];
        }
        v1 = val->str[i] == val->str[pos];
        v2 = tolower(val->str[i]) == tolower(val->str[pos]);
        printf("cal:%d  iter:%d\n", call, iter);
        fflush(stdout);
        // Конструкция с argv позволяет
        // выбирать: есть параметр --i или нет.
        if (!argv[0] && v1 || argv[0] && v2) {
            pos++;
        }
        pref[i] = pos;
        iter++;
    }
    call++;
    return pref;
}

// Поиск подстроки по КМП, argv[0] == i, argv[1] == v
int kmp(const string* tmp, const string* const text, int* argv) {

    string* worker = malloc(sizeof(string));
    int tmp_len = tmp->len;

    // Задаем строку, в которой ищем шаблон
    string_init(worker, tmp);
    append(worker, '#');
    cat_str(worker, text);

    int* pref = pref_func(worker, argv);
    print_array(pref, worker->len, stdout);

    for (int i = 0; i < text->len; i++) {
        if (pref[i + tmp_len + 1] == tmp_len) {
            if (argv[1]) {
                del_str(worker);
                return 0;
            } else {
                del_str(worker);
                return 1;
            }
        }
    }

    del_str(worker);
    if (argv[1]) {
        return 1;
    } else {
        return 0;
    }
}

void print_dir(string* dir_ptr) {
    DIR* dir = 0;
    struct dirent* direntp = 0;
    dir = opendir(dir_ptr->str);
    while ((direntp = readdir(dir)) != 0) {

        printf("%s\n", direntp->d_name);

    }
}

void print_dir_char(char* dir_ptr) {
    DIR* dir = 0;
    struct dirent* direntp = 0;
    dir = opendir(dir_ptr);
    while ((direntp = readdir(dir)) != 0) {

        printf("%s\n", direntp->d_name);

    }
}

void print_array(int* array, int size, FILE* file) {

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", array[i]);
    }
    fprintf(file, "\n");
    fflush(file);
}

#endif //STRINGS_GREP_H
