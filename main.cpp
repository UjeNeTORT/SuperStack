//TODO handle some shit that can happen if something

#include <stdio.h>

#include "stack.h"

int main() {
    stack cat = {};

    enum PUSH_OUT push_err = PUSH_NO_ERR;

    StackCtor(&cat, 1);

    StackPush(&cat,  1);
    StackPush(&cat,  2);
    StackPush(&cat,  3);
    StackPush(&cat,  4);
    StackPush(&cat,  5);
    StackPush(&cat,  6);

    StackPush(&cat,  7);

    StackPush(&cat,  8);
    StackPush(&cat,  9);
    StackPush(&cat, 10);
    StackPush(&cat, 11);
    StackPush(&cat, 12);
    StackPush(&cat, 13);
    StackPush(&cat, 14);
    StackPush(&cat, 15);
    StackPush(&cat, 16);
    StackPush(&cat, 17);
    StackPush(&cat, 18);

    enum POP_OUT pop_err = POP_NO_ERR;

    printf("%d\n", StackPop(&cat, &pop_err));
    // cat.data.buf[0] = 1000;
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));
    printf("%d\n", StackPop(&cat, &pop_err));

    Elem_t val = StackPop(&cat, &pop_err);
    if (pop_err == POP_NO_ERR)
        printf("%d\n", val);
    else
        printf("error in pop, please, launch in debug mode to see\n");


    StackDtor(&cat);

    printf("we are at the end! EEEEEEEOOOOOO\n");
    return 0;
}
