//TODO handle some shit that can happen if something

/*

TODO

[DONE] return int -> enum

rename StackX -> XStack so as not to type Stack every time u need some func

stack dump: fix ########

[DONE] capacity decreasing (we need capacity to never be less than initial capacity)
[DONE] init capacity

[DONE] make stderr printfs if somethings broken

add constant to manually increase / decrease stack capacity (now it is 2)

[DONE] StackPop - make argument to catch errors

[DONE] return POISON (read hex speak)

[DONE] max_stack may not be nessessary (calloc already bounds something - TOREAD)

poison is not portable on other Elem_ts - TOASK

[DONE] StackRealloc does not set other values to POISON - make func for set to poison

StackDtor memset musor before free

canary protect (FAT MAMA BIRD)
canary to условная компиляция

vscode configuration

[DONE] what if debug mode is off

log file with time in name

how to protect initial capacity?

local define (ask sanya)

[[noignore]]

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "colors.h"
#include "stack.h"

enum POISON_OUT {
    POISON_ERR_SIDE = -1,
    POISON_NO_ERR = 0,
    POISON_ERR = 1
};

static char * FormErrMsg         (unsigned err_vector);
static enum POISON_OUT StackPoison        (stack *stk);
// static int    GetNewCapacity     (int capacity, int max_capacity);
static void   PrintStackDataDump (FILE *fout, const stack *stk);

// in order not to create another one every time i need it
unsigned err_vector = 0;
char * err_msg = (char *) calloc(MAX_ERR_MSG_STRING, sizeof(char *));

//-------------------------------------------------------------------------------------
enum CTOR_OUT StackCtor(stack *stk, int capacity) {

    //dont use StackErr because no way it breaks
    assert(stk);

    if (!stk)
        return CTOR_OUT::CTOR_NULL_STK;

    stk->size = 0;
    stk->capacity = capacity;
    stk->init_capacity = capacity;

    if (capacity > 0) {

        stk->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));

        StackPoison(stk);
    }

    else {
        stk->data = NULL;
    }

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return CTOR_OUT::CTOR_ERR;

    return CTOR_OUT::CTOR_NO_ERR;
}

//-------------------------------------------------------------------------------------
enum REALLC_OUT StackRealloc(stack *stk) {

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return REALLC_OUT::REALLC_ERR_SIDE;

    int new_capacity = stk->capacity;

    if (stk->size <= (int) stk->capacity / 4 + 1 && stk->capacity > stk->init_capacity)
        new_capacity = stk->capacity / 2;

    if (stk->size >= (int) stk->capacity * 3 / 4)
        new_capacity = stk->capacity * 2;

    if (stk->capacity != new_capacity) {

        stk->data = (Elem_t *) realloc(stk->data, new_capacity * sizeof(Elem_t));
        stk->capacity = new_capacity;
    }

    StackPoison(stk);

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return REALLC_OUT::REALLC_ERR;

    return REALLC_OUT::REALLC_NO_ERR;
}

//-------------------------------------------------------------------------------------
enum DTOR_OUT StackDtor(stack *stk) {

    ASSERT_STACK(stk); // should we?

    if (StackErr(stk))
        return DTOR_OUT::DTOR_ERR_SIDE;

    stk->size = -1;
    stk->capacity = -1;

    // memset(stk->data, 0, stk->capacity); // what size should be?

    free(stk->data);

    err_vector = StackErr(stk);

    if (err_vector) return DTOR_OUT::DTOR_DESTR;

    STACK_DUMP(LOG_FILE, stk, err_vector);

    return DTOR_OUT::DTOR_NOT_DESTR;
}

//-------------------------------------------------------------------------------------
enum PUSH_OUT StackPush(stack *stk, Elem_t value) {

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return PUSH_ERR_SIDE;

    stk->data[stk->size] = value;
    stk->size++;

    StackRealloc(stk);

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return PUSH_ERR;

    return PUSH_NO_ERR;
}

//-------------------------------------------------------------------------------------
Elem_t StackPop(stack *stk, enum POP_OUT *err) {

    ASSERT_STACK(stk);

    if (StackErr(stk))
        *err = POP_ERR_SIDE;

    stk->size--;

    ASSERT_STACK(stk);

    if (StackErr(stk))
        *err = POP_ERR;
    else
        *err = POP_NO_ERR;

    return stk->data[stk->size]; // if not poison
}

//-------------------------------------------------------------------------------------
/*
what are the possibilities?

o MAKE BUFFER TO WHICH WE PUT STRINGS "*[0] = 10", ...
  AND MAKE MACROS-WRAPPER to print info like "data [0x001AD0C] {<text_from_buffer>}"
  + easy to do
  + training with macros
  - mamory consuming

o MAKE SEQUENCE OF FPRINTF
  + very easy to do
  - hard to change if i need it
  - big code
  - hard to debug
  - i dont work with macros (but i want to do macros as i dont feel confident while working with it)

*/
void StackDump(const char  * const fname,
               const stack *       stk,
               unsigned            err_vector,
               const char  *       stk_name,
               const char  * const err_file,
               int                 err_line,
               const char  * const err_func) {

    assert (stk);
    assert (fname);
    assert (stk_name);
    assert (err_file);
    assert (err_func);

    FILE *fout = fopen(fname, "w");

    assert (fout);

    fprintf(fout, "stack[%p] %s from %s (%d)\n"
                  "             called from %s (%d) %s\n", stk, stk_name, "########", -1, err_file, err_line, err_func);

    fprintf(fout, "{\n");

    fprintf(fout, "size     = %d (MX_STK = %d)\n", stk->size, MX_STK);
    fprintf(fout, "capacity = %d\n", stk->capacity);

    err_msg = FormErrMsg(err_vector);
    if (*err_msg)
        fprintf(fout, "ERRORS: %s", err_msg);

    PrintStackDataDump(fout, stk);

    fprintf(fout, "}\n");

    fclose(fout);
}

//-------------------------------------------------------------------------------------
//verificator
int StackErr(const stack *stk) {

    unsigned errors = 0;

    if (!stk)                               errors |=  1;
    if (!stk->data)                         errors |=  2;
    if (stk->size < 0)                      errors |=  4;
    if (stk->capacity <= 0)                 errors |=  8;
    if (stk->size >= stk->capacity)         errors |= 16;
    if (stk->capacity > MX_STK)             errors |= 32;
    if (stk->init_capacity > stk->capacity) errors |= 64;

    return errors;
}

//-------------------------------------------------------------------------------------
/*anscillary function for StackDump

is used to form error messages
*/
static char * FormErrMsg(unsigned err_vec) {

    strcpy(err_msg, "");

    if (err_vec &  1)
        ADD_ERR_MSG(err_msg, "stack null pointer");

    if (err_vec &  2)
        ADD_ERR_MSG(err_msg, "data null pointer");

    if (err_vec &  4)
        ADD_ERR_MSG(err_msg, "size < 0");

    if (err_vec &  8)
        ADD_ERR_MSG(err_msg, "capacity <= 0");

    if (err_vec & 16)
        ADD_ERR_MSG(err_msg, "size >= capacity");

    if (err_vec & 32)
        ADD_ERR_MSG(err_msg, "capacity bigger than max stack size");

    if (err_vec & 64)
        ADD_ERR_MSG(err_msg, "init capacity is bigger than capacity");

    strcat(err_msg, "\n");

    return err_msg;
}

//-------------------------------------------------------------------------------------
//TODO different modes: fast and memory-effective
// static int GetNewCapacity(int capacity, int max_capacity) {
//    TODO
// }

//-------------------------------------------------------------------------------------
/*
returns pointer to string which contents fully prepared string with every element of data


string which we return (1) can be contained in separate dynamically allocated reusable buffer
                       (2) can be compilation-time constant
        we use   option 2 because if we call StackDump, program stops and this means that we need to call GetStackDataDump only once
        usage of option 1 would be overengineering

if size of buffer <= 0 we return data[0x00000] { size < 0 nothing to look at}
if data == NULL        we return data[0x00000] { nothing to look at }
if data != NULL        we print  data[address] { *[0] = 10\n *[1] = 1\n and so on and so forth}


PROBLEM: how can we use format-strings outside of fprintf (to use strcat (..., "[%d] = %d\n", <%-values>))?
 */
static void PrintStackDataDump(FILE *fout, const stack *stk) {

    /* here we dont use ASSERT_STACK because we use this func in debug and this means that stack is already not ok */
    assert (stk);
    assert (fout);

    fprintf(fout, "data[0x%p]\n"
                  "\t{\n", stk->data);

    if (!stk->data) {
        fprintf(fout, "\t\tdata null pointer, nothing to look at\n");
    }
    else if (stk->capacity <= 0) {
        fprintf(fout, "\t\tstack capacity is 0 or less, cant print it\n");
    }
    else {

        for (int i = 0; i < stk->capacity; i++) {

            if (i < stk->size)
                fprintf(fout, "\t\t*[%4d] = %10d\n", i, stk->data[i]);
            else
                fprintf(fout, "\t\t [%4d] = %10d %s\n", i, stk->data[i], (stk->data[i] == POISON) ? "(POISON)" : "");

        }
    }

    fprintf(fout, "\t}\n");
}

static enum POISON_OUT StackPoison(stack *stk) {

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return POISON_ERR_SIDE;

    int cnt = stk->size;

    while (cnt < stk->capacity)
        stk->data[cnt++] = POISON;

    ASSERT_STACK(stk);

    if (StackErr(stk))
        return POISON_ERR;

    return POISON_NO_ERR;
}
