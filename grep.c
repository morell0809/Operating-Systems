//
// Created by morell on 06.03.17.
//
#include <getopt.h>
#include "strings.h"
#include "grep.h"

int main(int argc, char** argv) {


    int val;
    char c;
    const int count_opt = 2;
    int* options = malloc(sizeof(int) * count_opt);
    for (int i = 0; i < count_opt; i++) {
        options[i] = 0;
    }

    // Парсинг аргументов
    while ((val = getopt(argc, argv, "vi")) != -1) {
        switch (val) {
            case 'i':
                options[0] = 1;
                break;
            case 'v':
                options[1] = 1;
                break;
            case '?':
                c = val;
                printf("Illegal arg: %c", c);
                break;
        }
    }

    string* str_text = malloc(sizeof(string));
    char_init(str_text, "", BUFSIZ);
    // После разбора аргументов optind должен указывать на шаблон
    // Чтение stdin. optind == #аргументов

    if( optind == argc ) {

        printf("Usage: grep [OPTION]... PATTERN [FILE]...\n");

    } else if (optind + 1 < argc) {
        // Optind указывает на шаблон => начало с optind + 1
        for (int i = optind + 1; i < argc; i++) {
            FILE* input = fopen(argv[i], "r");
            if (!input) {

                perror("grep:");
                continue;

            }
            while (read_one_str(str_text, input) != -1) {
                string* tmp = malloc(sizeof(string));
                char_init(tmp, argv[optind], BUFSIZ);
                if (kmp(tmp, str_text, options)) {

                    printf("%s: %s\n", argv[i], str_text->str);

                }
            }
            fclose(input);
        }

    } else { // Иначе пробуем открывать поэтапно файлы

        while (read_one_str(str_text, 0) != -1) {
            string* tmp = malloc(sizeof(string));
            char_init(tmp, argv[optind], BUFSIZ);
            if (kmp(tmp, str_text, options)) {
                printf("stdout: %s\n", str_text->str);
            }
        }
    }
    del_str(str_text);
    return 0;
}
