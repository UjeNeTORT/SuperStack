//TODO handle some shit that can happen if something

#include <stdio.h>

#include "stack.h"


int main() {

    stack cat = {};

    StackCtor(&cat, 10);

    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);
    StackPush(&cat, 22848);

    printf("%d\n", StackPop(&cat));

    return 0;
}
