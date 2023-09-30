//TODO handle some shit that can happen if something

#include <stdio.h>

#include "stack.h"

int main() {
    stack cat = {};

    enum PUSH_OUT push_err = PUSH_NO_ERR;

    StackCtor(&cat, 1, DEBUG_INFO(cat));
    StackCtor(&cat, 1, DEBUG_INFO(cat));

    StackPush(&cat,  1, DEBUG_INFO(cat));
    StackPush(&cat,  2, DEBUG_INFO(cat));
    StackPush(&cat,  3, DEBUG_INFO(cat));
    StackPush(&cat,  4, DEBUG_INFO(cat));
    StackPush(&cat,  5, DEBUG_INFO(cat));
    StackPush(&cat,  6, DEBUG_INFO(cat));
    StackPush(&cat,  7, DEBUG_INFO(cat));
    StackPush(&cat,  8, DEBUG_INFO(cat));
    StackPush(&cat,  9, DEBUG_INFO(cat));
    StackPush(&cat, 10, DEBUG_INFO(cat));
    StackPush(&cat, 11, DEBUG_INFO(cat));
    StackPush(&cat, 12, DEBUG_INFO(cat));
    StackPush(&cat, 13, DEBUG_INFO(cat));
    StackPush(&cat, 14, DEBUG_INFO(cat));
    StackPush(&cat, 15, DEBUG_INFO(cat));
    StackPush(&cat, 16, DEBUG_INFO(cat));
    StackPush(&cat, 17, DEBUG_INFO(cat));
    StackPush(&cat, 18, DEBUG_INFO(cat));

    enum POP_OUT pop_err = POP_NO_ERR;

    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));
    printf("%d\n", StackPop(&cat, &pop_err, DEBUG_INFO(cat)));

    Elem_t val = StackPop(&cat, &pop_err, DEBUG_INFO(cat));
    if (pop_err == POP_NO_ERR)
        printf("%d\n", val);
    else
        printf("error in pop, please, launch in debug mode to see\n");


    StackDtor(&cat, DEBUG_INFO(cat));
    StackDtor(&cat, DEBUG_INFO(cat));

    printf("we are at the end! EEEEEEEOOOOOO\n");
    return 0;
}
