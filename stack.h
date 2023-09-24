//  c++ library is calles "stack", "not stack.h", so no naming conflicts

#include <limits.h>

#define DEBUG_MODE

#if (defined(DEBUG_MODE))

    #define STACK_DUMP(fname, stk, err_vector) StackDump((fname), (stk), (err_vector), (#stk), __FILE__, __LINE__, __FUNCTION__)

    #define ADD_ERR_MSG(prev, msg) strcat((*(prev)) ? strcat((prev), ", ") : (prev), (msg))

    // assume that err_vector declared before
    #define ASSERT_STACK(stk)                                \
    {                                                        \
        err_vector = StackErr(stk);                          \
        if (err_vector) {                                    \
            STACK_DUMP(LOG_FILE, stk, err_vector);           \
            fprintf(stderr, "Stack corrupted. ABORTED\n");   \
            abort();                                         \
        }                                                    \
    }

#else
    #define STACK_DUMP(fname, stk, err_vector) ;
    #define ADD_ERR_MSG(prev, msg)             ;
    #define ASSERT_STACK(stk)                  ;
#endif

const char * const LOG_FILE = "log.log";

const int    MX_STK             = 100;
const int    POISON             = 0xD00D1E;
const size_t MAX_ERR_MSG_STRING = 400;

typedef int Elem_t;

struct stack {
    Elem_t* data;
    int size;
    int capacity;
    int init_capacity;
};

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

enum CTOR_OUT
StackCtor(stack *stk,
          int    capacity);

enum REALLC_OUT
StackRealloc(stack *stk);

enum DTOR_OUT
StackDtor(stack *stk);

enum PUSH_OUT
StackPush(stack *stk,
          Elem_t value);

Elem_t
StackPop(stack   *     stk,
         enum POP_OUT *err);

void
StackDump(const char  * const fname,
          const stack *       stk,
          unsigned            err_vector,
          const char  * const stk_name,
          const char  * const err_file,
          int                 err_line,
          const char  * const err_func);

int StackErr(const stack *stk);
