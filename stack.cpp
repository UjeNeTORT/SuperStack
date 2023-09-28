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

how to protect initial capacity? - by hash

local define (ask sanya)

[FUNNY] stack dump + dump for dump + dump for dump for dump + dump for dump for dump for dump (lol)

add calloc checks in Stackcalloc

[DONE] lc + data + rc to struct

[[noignore]]

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "colors.h"
#include "my_hash.h"
#include "stack.h"

enum POISON_OUT {
    POISON_ERR_SIDE = -1,
    POISON_NO_ERR   =  0,
    POISON_ERR      =  1
};

enum DATA_CLLC_OUT {
    DATA_CLLC_ERR_SIDE = -1,
    DATA_CLLC_NO_ERR   =  0,
    DATA_CLLC_ERR      =  1
};

enum DATA_RLLC_OUT {
    DATA_RLLC_ERR_SIDE = -1,
    DATA_RLLC_NO_ERR   =  0,
    DATA_RLLC_ERR      =  1
};

static enum DATA_CLLC_OUT
              StackDataCalloc    (stk_data * data, int capacity);

static enum DATA_RLLC_OUT
              StackDataRealloc   (stk_data * data, int new_capacity);

static enum POISON_OUT
              StackPoison        (stack *stk);

static void   FreeData           (stk_data *data, int capacity);

static char * FormErrMsg         (size_t err_vector);

static void   PrintStackDataDump (FILE *fout, const stack *stk);

static int    GetNewCapacity     (stack *stk);

// in order not to create another one every time i need it
unsigned err_vector = 0;
char * err_msg = (char *) calloc(MAX_ERR_MSG_STRING, sizeof(char *));

//-------------------------------------------------------------------------------------
enum CTOR_OUT StackCtor(stack *stk, int capacity) {

    //dont use StackErr because we are in constructor and no way it breaks
    assert(stk);
    if (!stk)
        return CTOR_NULL_STK;

    #if (defined(STACK_CANARY_PROTECT))

    stk->l_canary = LEFT_CHICK;
    stk->r_canary = RIGHT_CHICK;

    #endif // defined(STACK_CANARY_PROTECT)

    stk->size = 0;
    stk->capacity      = capacity;
    stk->init_capacity = capacity;

    StackDataCalloc(&stk->data, capacity);

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return CTOR_ERR;

    StackPoison(stk);

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return CTOR_ERR;

    return CTOR_NO_ERR;
}

//-------------------------------------------------------------------------------------
enum REALLC_OUT StackRealloc(stack *stk, int new_capacity) {

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return REALLC_ERR_SIDE;

    if (stk->capacity != new_capacity) {

        StackDataRealloc(&stk->data, new_capacity);
        stk->capacity = new_capacity;
    }

    StackPoison(stk);

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return REALLC_ERR;

    return REALLC_NO_ERR;
}

//-------------------------------------------------------------------------------------
enum DTOR_OUT StackDtor(stack *stk) {

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return DTOR_ERR_SIDE;

    #if (defined(STACK_CANARY_PROTECT))

    stk->l_canary = 0;
    stk->r_canary = 0;

    #endif // defined(STACK_CANARY_PROTECT)

    stk->size     = -1;
    stk->capacity = -1;
    FreeData(&stk->data, stk->capacity);

    stk = NULL; // didnt test

    return DTOR_DESTR;
}

//-------------------------------------------------------------------------------------
enum PUSH_OUT StackPush(stack *stk, Elem_t value) {

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return PUSH_ERR_SIDE;

    stk->data.buf[stk->size] = value;
    stk->size++;

    int new_capacity = GetNewCapacity(stk);

    if (stk->capacity != new_capacity) {
        StackRealloc(stk, new_capacity);

    }

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
    Elem_t ret_value = stk->data.buf[stk->size];

    ASSERT_STACK(stk);
    if (StackErr(stk))
        *err = POP_ERR;
    else
        *err = POP_NO_ERR;

    int new_capacity = GetNewCapacity(stk);

    if (stk->capacity != new_capacity) {
        StackRealloc(stk, new_capacity);
    }

    ASSERT_STACK(stk);
    if (StackErr(stk))
        *err = POP_ERR;
    else
        *err = POP_NO_ERR;

    return ret_value;
}

//-------------------------------------------------------------------------------------
static enum DATA_CLLC_OUT StackDataCalloc (stk_data * data, int capacity) {

    assert (data);

    if (!data)
        return DATA_CLLC_ERR_SIDE;

    if (capacity <= 0) {

        #if (defined(DATA_CANARY_PROTECT))

            data->l_canary = NULL;
            data->r_canary = NULL;

        #endif // defined(DATA_CANARY_PROTECT)

        data->buf = NULL;

        return DATA_CLLC_ERR_SIDE;
    }
    else {

        #if (defined(DATA_CANARY_PROTECT))

        Elem_t * init_buf_ptr = (Elem_t * ) calloc(capacity * sizeof(Elem_t) + 2 *sizeof(Canary_t), 1);

        if (!init_buf_ptr)
            return DATA_CLLC_ERR;

        data->l_canary = (Canary_t *) init_buf_ptr;
       *data->l_canary = LEFT_CHICK;

        data->buf = (Elem_t * ) ((Canary_t * ) init_buf_ptr + 1);

        data->r_canary = (Canary_t * ) (data->buf + capacity);
       *data->r_canary = RIGHT_CHICK;

        #else

        data->buf = (Elem_t *) calloc(capacity, sizeof(Elem_t));

        if (!data->buf)
            return DATA_CLLC_ERR;

        #endif // defined(DATA_CANARY_PROTECT)
    }

    return DATA_CLLC_NO_ERR;
}

//-------------------------------------------------------------------------------------
static enum DATA_RLLC_OUT StackDataRealloc (stk_data * data, int new_capacity) {

    assert(data);
    if (!data)
        return DATA_RLLC_ERR_SIDE;

    #if (defined(DATA_CANARY_PROTECT))

    data->buf = (Elem_t * ) realloc(data->l_canary, new_capacity * sizeof(Elem_t) + 2 * sizeof(Canary_t)); // CAREFULLY WITH CANARIES

    if (!data->buf)
        return DATA_RLLC_ERR;

    data->l_canary = (Canary_t * ) data->buf;
   *data->l_canary = LEFT_CHICK;

    data->buf = (Elem_t *) ((Canary_t *) data->buf + 1);

    data->r_canary = (Canary_t * ) (data->buf + new_capacity);
   *data->r_canary = RIGHT_CHICK;

    #else

    data->buf = (Elem_t * ) realloc(data->buf, new_capacity);

    assert(data->buf);
    if (!data->buf)
        return DATA_RLLC_ERR;

    #endif // defined(DATA_CANARY_PROTECT)

    return DATA_RLLC_NO_ERR;
}

//-------------------------------------------------------------------------------------
static void FreeData (stk_data *data, int capacity) {

    for (size_t i = 0; i < capacity; i++) {
        data->buf[i] = POISON; // DEAD MEMORY
    }

    data->buf = NULL;

    #if (defined(DATA_CANARY_PROTECT))

        *data->l_canary = 0;
        *data->r_canary = 0;

         free(data->l_canary);

         data->l_canary = NULL;
         data->r_canary = NULL;

    #endif // defined(DATA_CANARY_PROTECT)
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
               size_t              err_vector,
               const char  *       stk_name,
               const char  * const err_file,
               int                 err_line,
               const char  * const err_func) {

    assert (stk);
    assert (fname);
    assert (stk_name);
    assert (err_file);
    assert (err_func);

    FILE *fout = fopen(fname, "a");

    assert (fout);

    fprintf(fout, "stack[%p] %s from %s (%d)\n"
                  "             called from %s (%d) %s\n", stk, stk_name, "########", -1, err_file, err_line, err_func);
    fprintf(fout, "{\n");

    #if (defined(STACK_CANARY_PROTECT))

    fprintf(fout, "left  stack canary[%p] = %lu (%s)\n", &stk->l_canary, stk->l_canary, (stk->l_canary == LEFT_CHICK)  ? "ok" : "corrupted");
    fprintf(fout, "right stack canary[%p] = %lu (%s)\n", &stk->r_canary, stk->r_canary, (stk->r_canary == RIGHT_CHICK) ? "ok" : "corrupted");

    #endif // defined(STACK_CANARY_PROTECT)

    fprintf(fout, "size     = %d (MX_STK = %d)\n", stk->size, MX_STK);
    fprintf(fout, "capacity = %d\n", stk->capacity);

    err_msg = FormErrMsg(err_vector);
    if (*err_msg)
        fprintf(fout, "ERRORS: %s\n", err_msg);

    PrintStackDataDump(fout, stk);

    fprintf(fout, "}\n");

    fclose(fout);
}

//-------------------------------------------------------------------------------------
//verificator
size_t StackErr(const stack *stk) {

    size_t errors = 0;

    if (!stk)                               errors |=    1;
    if (!stk->data.buf)                     errors |=    2;
    if (stk->size < 0)                      errors |=    4;
    if (stk->capacity <= 0)                 errors |=    8;
    if (stk->size >= stk->capacity)         errors |=   16;
    if (stk->capacity > MX_STK)             errors |=   32;
    if (stk->init_capacity > stk->capacity) errors |=   64;

    #if (defined(DATA_CANARY_PROTECT))

    if (!stk->data.l_canary || *stk->data.l_canary != LEFT_CHICK )
                                            errors |=  128;

    if (!stk->data.r_canary || *stk->data.r_canary != RIGHT_CHICK)
                                            errors |=  256;

    #endif // defined(DATA_CANARY_PROTECT)

    #if (defined(STACK_CANARY_PROTECT))

    if (stk->l_canary != LEFT_CHICK )       errors |=  512;
    if (stk->r_canary != RIGHT_CHICK)       errors |= 1024;

    #endif // defined(STACK_CANARY_PROTECT)

    return errors;
}

//-------------------------------------------------------------------------------------
/*anscillary function for StackDump

is used to form error messages
*/
static char * FormErrMsg(size_t err_vec) {

    strcpy(err_msg, "");

    if (err_vec &  1)
        ADD_ERR_MSG(err_msg, "stack null pointer");

    if (err_vec &  2)
        ADD_ERR_MSG(err_msg, "buf null pointer");

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

    #if (defined(DATA_CANARY_PROTECT))

    if (err_vec & 128)
        ADD_ERR_MSG(err_msg, "left canary: data attacked from the left");

    if (err_vec & 256)
        ADD_ERR_MSG(err_msg, "right canary: data attacked from the right");

    #endif // defined(DATA_CANARY_PROTECT)

    #if (defined(STACK_CANARY_PROTECT))

    if (err_vec & 512)
        ADD_ERR_MSG(err_msg, "left canary: stack attacked from the left");

    if (err_vec & 1024)
        ADD_ERR_MSG(err_msg, "right canary: stack attacked from the right");

    #endif // defined(STACK_CANARY_PROTECT)

    strcat(err_msg, "\n");

    return err_msg;
}

//-------------------------------------------------------------------------------------
static int GetNewCapacity(stack *stk) {

    if (stk->size <= (int) stk->capacity / 4 + 1 && stk->capacity > stk->init_capacity)
        return stk->capacity / 2;

    if (stk->size >= (int) stk->capacity * 3 / 4)
        return stk->capacity * 2;

    return stk->capacity;
}

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

    #if (defined(DATA_CANARY_PROTECT))

    fprintf(fout, "l_canary[%p] = %lu (%s)\n", stk->data.l_canary, (stk->data.l_canary) ? *stk->data.l_canary : 0, (stk->data.l_canary) ? ((*stk->data.l_canary == LEFT_CHICK)  ? "ok" : "corrupted") : "ptr null");
    fprintf(fout, "r_canary[%p] = %lu (%s)\n", stk->data.r_canary, (stk->data.r_canary) ? *stk->data.r_canary : 0, (stk->data.r_canary) ? ((*stk->data.r_canary == RIGHT_CHICK) ? "ok" : "corrupted") : "ptr null");

    #endif // defined(DATA_CANARY_PROTECT)

    fprintf(fout, "buf[%p]\n"
                  "\t{\n", stk->data.buf);

    if (!stk->data.buf) {
        fprintf(fout, "\t\tbuf null pointer, nothing to look at\n");
    }
    else if (stk->capacity <= 0) {
        fprintf(fout, "\t\tstack capacity is 0 or less, cant print it\n");
    }
    else {

        for (int i = 0; i < stk->capacity; i++) {

            if (i < stk->size)
                fprintf(fout, "\t\t*[%4d] = %10d\n", i, stk->data.buf[i]);
            else
                fprintf(fout, "\t\t [%4d] = %10d %s\n", i, stk->data.buf[i], (stk->data.buf[i] == POISON) ? "(POISON)" : "");

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
        stk->data.buf[cnt++] = POISON;

    ASSERT_STACK(stk);
    if (StackErr(stk))
        return POISON_ERR;

    return POISON_NO_ERR;
}
