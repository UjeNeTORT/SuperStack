//  c++ library is called "stack", "not stack.h", so no naming conflicts

#include <limits.h>

#include "my_hash.h"

#define DEBUG_MODE

#define STACK_CANARY_PROTECT
#define DATA_CANARY_PROTECT

#define STACK_HASH_PROTECT
#define DATA_HASH_PROTECT

//-------------------------------------------------------------------------------------
typedef int    Elem_t;
typedef size_t Canary_t;

//-------------------------------------------------------------------------------------
#if (defined(DEBUG_MODE))

    #define STACK_DUMP(fname, stk, err_vector) StackDump((fname), (stk), (err_vector), (#stk), __FILE__, __LINE__, __FUNCTION__)

    #define ADD_ERR_MSG(prev, msg) strcat((*(prev)) ? strcat((prev), ", ") : (prev), (msg))

    // assume that err_vector declared before
    #define ASSERT_STACK(stk, call_from)                     \
    {                                                        \
        err_vector = StackErr(stk, call_from);               \
        if (err_vector) {                                    \
            STACK_DUMP(LOG_FILE, stk, err_vector);           \
            fprintf(stderr, "Stack corrupted. ABORTED\n");   \
            abort();                                         \
        }                                                    \
    }

#else
    #define STACK_DUMP(fname, stk, err_vector) ;
    #define ADD_ERR_MSG(prev, msg)             ;
    #define ASSERT_STACK(stk, call_from)       ;

#endif // defined(DEBUG_MODE)

//-------------------------------------------------------------------------------------
// i mean canary protect. I have right to laugh while coding, dont look at me like that
#if (defined(STACK_CANARY_PROTECT) || defined(DATA_CANARY_PROTECT))
    const Canary_t LEFT_CHICK  = 0xB38F0F83F03F80AA; //  in 0b it looks like 101100111000...10101010
    const Canary_t RIGHT_CHICK = 0xB38F0F83F03F80AA; //  in 0b it looks like 101100111000...10101010
#else

#endif // defined(defined(STACK_CANARY_PROTECT) || defined(DATA_CANARY_PROTECT))

//-------------------------------------------------------------------------------------
const char * const LOG_FILE = "log.log";

const int    MX_STK             = 100;
const int    POISON             = 0xD00D1E;
const size_t MAX_ERR_MSG_STRING = 400;

//-------------------------------------------------------------------------------------
struct stk_data {

    Elem_t *buf;

    #if (defined(DATA_CANARY_PROTECT))

    Canary_t *l_canary;
    Canary_t *r_canary;

    #endif // defined(DATA_CANARY_PROTECT)

    #if (defined(DATA_HASH_PROTECT))

    Hash_t    hash_sum;

    #endif // defined(DATA_HASH_PROTECT)
};

//-------------------------------------------------------------------------------------
struct stack {

    #if (defined(STACK_CANARY_PROTECT))

    Canary_t        l_canary;

    #endif // defined(STACK_CANARY_PROTECT)

    stk_data        data;
    int             size;
    int             capacity;      // int instead of size_t in order to catch errors
    int             init_capacity;

    #if (defined(STACK_HASH_PROTECT))
    Hash_t    *     hash_sum;
    #endif // defined(STACK_HASH_PROTECT)

    #if (defined(STACK_CANARY_PROTECT))

    Canary_t        r_canary;

    #endif // defined(STACK_CANARY_PROTECT)
};

//-------------------------------------------------------------------------------------
enum CTOR_OUT {
    CTOR_NULL_STK = -1,
    CTOR_NO_ERR   =  0,
    CTOR_ERR      =  1
};

enum REALLC_OUT {
    REALLC_ERR_SIDE = -1,
    REALLC_NO_ERR   =  0,
    REALLC_ERR      =  1
};

enum DTOR_OUT {
    DTOR_ERR_SIDE  = -1,
    DTOR_DESTR     =  0,
    DTOR_NOT_DESTR =  1

};

enum PUSH_OUT {
    PUSH_ERR_SIDE = -1,  // if function received already corrupted stack
    PUSH_NO_ERR   =  0,
    PUSH_ERR      =  1
};

enum POP_OUT {
    POP_ERR_SIDE = -1,  // if function received already corrupted stack
    POP_NO_ERR   =  0,
    POP_ERR      =  1
};

enum CALL_FROM {
    F_BEGIN = -1,
    F_MID   =  0,
    F_END   =  1
};

//-------------------------------------------------------------------------------------
enum CTOR_OUT
StackCtor(stack *stk,
          int    capacity);

//-------------------------------------------------------------------------------------
enum REALLC_OUT
StackRealloc(stack *stk, int new_capacity);

//-------------------------------------------------------------------------------------
enum DTOR_OUT
StackDtor(stack *stk);

//-------------------------------------------------------------------------------------
enum PUSH_OUT
StackPush(stack *stk,
          Elem_t value);

//-------------------------------------------------------------------------------------
Elem_t
StackPop(stack   *     stk,
         enum POP_OUT *err);

//-------------------------------------------------------------------------------------
void
StackDump(const char  * const fname,
          const stack *       stk,
          size_t              err_vector,
          const char  * const stk_name,
          const char  * const err_file,
          int                 err_line,
          const char  * const err_func);

//-------------------------------------------------------------------------------------
size_t StackErr(stack *stk, enum CALL_FROM call_from);
