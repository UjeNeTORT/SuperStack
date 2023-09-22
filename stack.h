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

const int MX_STK = 100;
const int POISON = 0xD00D1E;
const size_t MAX_ERR_MSG_STRING = 400;

typedef int Elem_t;

//TODO add critical delta = capacity - size where we realloc buffer
struct stack {
    Elem_t* data;
    int size;
    int capacity;
};


int StackCtor(stack *stk,
              int    capacity);

int StackRealloc(stack *stk,
                 int    new_capacity);

int StackDtor(stack *stk);

int StackPush(stack *stk,
              Elem_t value);

int StackPop(stack *stk);

int StackDump(const char  * const fname,
              const stack *       stk,
              unsigned            err_vector,
              const char  * const stk_name,
              const char  * const err_file,
              int                 err_line,
              const char  * const err_func);

int StackErr(const stack *stk);




