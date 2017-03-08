//
// Created by morell on 08.03.17.
//

#ifndef STRINGS_TEST_H
#define STRINGS_TEST_H

#include "strings.h"
#include "grep.h"

void test_func() {

    int *arg = malloc(sizeof(int) * 10);
    arg[0] = 1;
    arg[1] = 1;

    string *text = malloc(sizeof(string));
    char_init(text, "Hello  myfriend", BUFSIZ);
    int* pref = pref_func(text,arg);
    print_array(pref,text->len,stdout);
    string *tmp = malloc(sizeof(string));
    char_init(tmp, "hello", BUFSIZ);
    int* before = arg;


    int yes = kmp(tmp, text, arg);
    printf("yes: %d", yes);
    fflush(stdout);
}

#endif //STRINGS_TEST_H
