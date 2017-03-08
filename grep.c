//
// Created by morell on 06.03.17.
//
#include "strings.h"
#include "grep.h"
#include "test.h"


int main(int argc, char** argv) {

    string* input = malloc(sizeof(string));
    string* path = malloc(sizeof(string));
    string* str_tmp = malloc(sizeof(string));

    // Вектор файлов на чтение. path[0] == 0 => читаем с stdin
    int count_path =  1;
    FILE** from = malloc(sizeof(FILE*) * count_path);
    from[0] = 0;
    read_one_str(input, from);
    read_one_str(path, from);
    read_one_str(str_tmp, from);

    // Аргументы
    const int count_opt = 2;
    int* options = malloc(sizeof(int) * count_opt);
    for (int i = 0; i < input->len; i++) {
        switch (input->str[i]) {
            case 'i':
                printf("i");
                options[0] = 1;
                break;
            case 'v':
                printf("v");
                options[1] = 1;
                break;
            default :
                break;
        }
    }

    FILE* path_worker = fopen(path->str, "r+");
    string* str_text = malloc(sizeof(string));
    char_init(str_text, "", BUFSIZ);
    from[0] = path_worker;

    print_array(options, count_opt, stdout);
    while (read_one_str(str_text, from) != -1) {
        if (kmp(str_tmp, str_text, options)) {
            printf("%s\n", str_text->str);
        }
    }

    return 0;
}
