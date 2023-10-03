//TODO handle some shit that can happen if something

#include <stdio.h>

#include "stack.h"

int main() {
    stack cat = {};

    enum PUSH_OUT push_err = PUSH_NO_ERR;

    StackCtor(&cat, 1, DEBUG_INFO(cat));

    for (int i = 0; i < 20; i++)
        PushStack(&cat, i);

    enum POP_OUT pop_err = POP_NO_ERR;

    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));
    printf("%d\n", PopStack(&cat, &pop_err));



    Elem_t val = PopStack(&cat, &pop_err);
    if (pop_err == POP_NO_ERR)
        printf("%d\n", val);
    else
        printf("error in pop, please, launch in debug mode to see\n");


    StackDtor(&cat, DEBUG_INFO(cat));

    printf("we are at the end! EEEEEEEOOOOOO\n");
    return 0;
}
