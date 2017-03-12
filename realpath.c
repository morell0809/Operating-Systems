//
// Created by morell on 19.02.17.
//

#include <stdio.h>
#include "realpath.h"
#include "strings.h"


// Пропускает все delim version = 1, выравнить на последний version = 0
int skip_delim(string*, int, char, int version);

// Выравнивает на след. delim
int next_delim(string* res, int pos, char delim);

// Нижн.путь
void pwddown(string* res, string* parse_path, int pos);

//1. Проетестить argv -
//2. Понять как добавить home
//3. lstat вместе с chdir
//4. В старой реализации добавить lstat
int main( int argc, char* argv[]) {

    int i = 1;
    argc = 2;
    string* input = (string*) malloc(sizeof(string));
    if (argc > 1) {
        read_str(input);
            string* parse_path = (string*) malloc(sizeof(string));
            string* realdown = (string*) malloc(sizeof(string));
            string_init(parse_path, input);
            // Идем не с корня
            if (input->str[0] != '/') {
                string* realup = (string*) malloc(sizeof(string));
                char_init(realup,"",BUFSIZ);
                pwd(get_inode("."),realup);
                reverse(realup);

                char_init(realdown,"/",BUFSIZ);
                //char_init(realdown, argv[0], strlen(argv[0]) + 1);
                append(realup, '/');
                cat_str(realup,parse_path);
             //   remove_back(realdown);
                // Идем вниз. Находим действительный путь.
                pwddown(realdown, realup, 0);
                del_str(realup);
                // В realdown будет итоговый путь.
                print(realdown);

            } else {// С корня
                char_init(realdown,"/",strlen(input->str) + 1);
                //  Идем вниз.Находим действительный путь.
                pwddown(realdown, parse_path, 0);
                print(realdown);
            }
            del_str(realdown);
            del_str(parse_path);
            i++;
    } else {

        printf("%s\n", "realpath: missing operand");

    }
    return 0;
}


