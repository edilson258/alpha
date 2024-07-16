#ifndef C_THINGS
#define C_THINGS

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>

/*
 * Typedef: Any_t
 *
 * Is just a wrapper of `void *`
 *
 */
typedef void *Any_t;

/*
 * Typedef: Node
 *
 * A wrapper of data on dynamic growing structures
 *
 */
typedef struct Node {
  void *data;
  struct Node *next;
} Node;

/*
 * Typedef: Node_t
 *
 * Is type alias of `Node *`
 *
 */
typedef Node *Node_t;

/*
 * Function: Node_new(Any_t data) -> Node_t
 *
 * Will returns the `data` and returns it wrapped in a `Node_t`
 *
 * Parameters:
 *
 * - Any_t data: a data to be wrapped
 *
 * Returns:
 *
 * - Node_t: a node that wraps the `data`
 *
 */
Node_t Node_new(Any_t data);

/*
 * Function: Int(int immediate) -> int*
 *
 * Will allocate the integer value `immediate` on the heap and return the
 * pointer
 *
 * Parameters:
 *
 * - int immediate: an immediate int value
 *
 * Returns:
 *
 * - int *: a pointer to the heap allocated integer `immediate`
 *
 */
int *Int(int immediate);

/*
 * Function: Str(char* immediate) -> char*
 *
 * Will allocate the string value `immediate` on the heap and return the
 * pointer
 *
 * Parameters:
 *
 * - char* immediate: an immediate string value
 *
 * Returns:
 *
 * - char *: a pointer to the heap allocated string `immediate`
 *
 */
char *Str(char *immediate);

/// Boolean Type
typedef enum {
  False = 0,
  True = 1,
} Boolean;

/// Convert a Boolean val to corresponding text
#define Boolean(x) x == False ? "False" : "True"

/*
 * Typedef: Str_t
 *
 * A type alias of 'char *'
 *
 */
typedef char *Str_t;




typedef struct String {
  /* Private field should only be accessed by internal API */
  char *inner;
  size_t length;
  pthread_mutex_t mutx;
} String;

/* Syntax sugar for `String *` */
typedef String *String_t;

/*
 * Function: String(text)
 *
 * Create a stack String instance initialized with the provided text.
 *
 * Parameters:
 *   text - Pointer to the C-style string to initialize the String with.
 *
 * Returns:
 *   The created String instance.
 */
#define String(text)                                                           \
  ({                                                                           \
    String string = {.inner = text};                                           \
    string;                                                                    \
  })

/*
 * Function: String_new(text)
 *
 * Create a new heap-allocated String instance initialized with the provided
 * text.
 *
 * Parameters:
 *   text - Pointer to the C-style string to initialize the String with.
 *
 * Returns:
 *   Pointer to the newly allocated String instance, or NULL if allocation
 * fails.
 *
 */
String_t String_new(Str_t text);

/*
 * Function: String_drop(string)
 *
 * Clean up (free memory) allocated for the String instance.
 *
 * Parameters:
 *   string - Pointer to the String instance to be cleaned up.
 *
 * Returns:
 *
 *   0 on success and -1 on error
 *
 */
int String_drop(String_t string);

/*
 * Function: String_val(string)
 *
 * Returns the raw C-style string (`char *`) stored inside the String.
 *
 * Parameters:
 *   string - Pointer to the String instance.
 *
 * Returns:
 *   Pointer to the C-style string contained within the String.
 *
 */
Str_t String_val(String_t string);

/*
 * Function: String_clone(src)
 *
 * Create and return a deep copy of the source String instance.
 *
 * Parameters:
 *   src - Pointer to the source String instance to be cloned.
 *
 * Returns:
 *   Pointer to the newly created deep copy of the source String instance.
 *
 */
String_t String_clone(String_t src);

/*
 * Function: String_ltrim(string)
 *
 * Remove leading whitespaces (including `\t, \f, \n, ...`) from the String.
 *
 * Parameters:
 *   string - Pointer to the String instance to be left-trimmed.
 *
 * Returns:
 *   Pointer to the left-trimmed String instance.
 *
 */
String_t String_ltrim(String_t string);

/*
 * Function: String_rtrim(string)
 *
 * Remove trailing whitespaces (including `\t, \f, \n, ...`) from the String.
 *
 * Parameters:
 *   string - Pointer to the String instance to be right-trimmed.
 *
 * Returns:
 *   Pointer to the right-trimmed String instance.
 *
 */
String_t String_rtrim(String_t string);

/*
 * Function: String_trim(string)
 *
 * Remove leading and trailing whitespaces (including `\t, \f, \n, ...`) from
 * the String.
 *
 * Parameters:
 *   string - Pointer to the String instance to be trimmed.
 *
 * Returns:
 *   Pointer to the trimmed String instance.
 *
 */
String_t String_trim(String_t string);

/*
 * Function: String_len(string)
 *
 * Returns the length of the C-style string stored inside the String instance.
 *
 * Parameters:
 *   string - Pointer to the String instance.
 *
 * Returns:
 *   Length of the C-style string stored inside the String.
 *
 */
size_t String_len(String_t string);

/*
 * Function: String_join(lhs, rhs)
 *
 * Join two String instances together and return a new String instance
 * containing the concatenated result.
 *
 * Parameters:
 *   lhs - Pointer to the left-hand side String instance.
 *   rhs - Pointer to the right-hand side String instance.
 *
 * Returns:
 *   Pointer to the newly created String instance containing the concatenated
 * result.
 *
 */
String_t String_join(String_t lhs, String_t rhs);

/*
 * Function: String_join_by(lhs, rhs, delimiter)
 *
 * Join two String instances together with a delimiter and return a new String
 * instance containing the concatenated result.
 *
 * Parameters:
 *   lhs - Pointer to the left-hand side String instance.
 *   rhs - Pointer to the right-hand side String instance.
 *   delimiter - Pointer to the delimiter C-style string to be used in between
 * lhs and rhs.
 *
 * Returns:
 *   Pointer to the newly created String instance containing the concatenated
 * result with the delimiter.
 *
 */
String_t String_join_by(String_t lhs, String_t rhs, Str_t delimiter);




/*
 * Typedef: Vector
 *
 * Represents a 1-D Vector
 *
 * `Note`: Every vector operations don't make any kind of memory allocation
 * or deallocation of the elements data. Thus, the developer must take care of
 * that to avoid corruptions of the stored element's data
 *
 */
typedef struct {
  // Head and tail of the linked list
  Node_t head;
  Node_t tail;

  Node_t head_iter;

  // number of elements
  size_t size;

  pthread_mutex_t mutx;
} Vector;

/*
 * Typedef: Vector_t
 *
 * Is just a type alias for `Vector *`
 *
 */
typedef Vector *Vector_t;

/*
 * Function: Vector_new() -> Vector_t
 *
 * Will create a new `Vector` instance on the heap
 *
 * Returns:
 *
 * -`Vector_t` a pointer to the Vector
 *
 */
Vector_t Vector_new();

/*
 * Function: Vector_isempty(Vector_t vec) -> int
 *
 * Checks if Vector `vec` is empty
 *
 * Parameters:
 *
 * - Vector_t vec: The vector to be checked if is empty
 *
 * Returns:
 *
 * - 1: when is empty
 * - 0: when is not empty
 *
 */
int Vector_isempty(Vector_t vec);

/*
 * Function: Vector_push(Vector_t vec, Any_t elem)
 *
 * Will append `elem` to the end of the Vector `vec`
 *
 * `Note`: Vector is implemented as linked list. So, this operation is O(n)
 *
 * Parameters:
 *
 * - Vector_t vec: The vector to push on
 * - Any_t elem: The element to pushed
 *
 */
void Vector_push(Vector_t vec, Any_t elem);

/*
 * Typedef: MapFn
 *
 * Is a function signature expected by the `Vector_map` function
 *
 * `Note`: This function must clone the underlyning data
 *
 */
typedef Any_t (*MapFn)(Any_t vec_item);

/*
 * Typedef: ReduceFn
 *
 * Is a function signature expected by the `Vector_reduce` function
 *
 */
typedef void (*ReduceFn)(Any_t vec_item, Any_t acc);

/*
 * Typedef: FilterFn
 *
 * Is a function signature expected by the `Vector_filter` function
 *
 * `Note`: This function must clone the elements
 *
 */
typedef int (*FilterFn)(Any_t vec_item);

/*
 * Function: Vector_map(Vector_t vec, MapFn fn) -> Vector_t
 *
 * Will map each elem in the Vector `vec` to Map Function `fn` and then return
 * the resulting Vector
 *
 * `Note`: The MapFn `fn` must clone the elements
 *
 * Parameters:
 *
 * - Vector_t vec: The original vector
 * - MapFn fn: The map function
 *
 * Returns:
 *
 * - Vector_t: A new vector with mapped elements
 *
 */
Vector_t Vector_map(Vector_t vec, MapFn fn);

/*
 * Function: Vector_reduce(Vector_t vec, ReduceFn fn, Any_t acc) -> acc
 *
 * Will map each element of the Vector `vec` to the Reduce Function `fn` that
 * way reducing the elements into the Accumulator `acc`
 *
 * Parameters:
 *
 * - Vector_t vec: Vector of elements to be reduced
 * - ReduceFn fn: The reduce function
 * - Any_t acc: The Accumulator
 *
 * Returns:
 *
 * - Any_t acc: The accumulated result
 *
 */
void Vector_reduce(Vector_t vec, ReduceFn fn, Any_t acc);

/*
 * Function: Vector_filter(Vector_t vec, FilterFn fn) -> Vector_t
 *
 * Will filter the elements in Vector `vec` according to the Filter Function
 * `fn`
 *
 * Parameters:
 *
 * - Vector_t vec: A vector to be filtered
 * - FilterFn fn: A predicate function
 *
 * Returns:
 *
 * - Vector_t vec: A vector with filtered elements
 *
 */
Vector_t Vector_filter(Vector_t vec, FilterFn fn);

/*
 * Function: Vector_len(Vector_t vec) -> size_t
 *
 * Will return the length of the Vector `vec`
 *
 * Parameters:
 *
 * - Vector_t vec: The vector to get its length
 *
 * Returns:
 *
 * - size_t: the length of the vector `vec`
 *
 */
size_t Vector_len(Vector_t vec);

/*
 * Function: Vector_at(Vector_t vec, size_t index) -> Any_t
 *
 * Will return the element in the index `index`, in case of out of bound `NULL`
 * will be returned
 *
 * `Note`: The element returned still pointed by the vector, the developer
 * should clone it before any modification to avoid data corruptions
 *
 * Parameters:
 *
 * - Vector_t vec: The Vector that holds the element to be indexed
 * - size_t index: The positio of the element in the vector
 *
 * Returns:
 *
 * - 'Any_t': the element or NULL in case of out of bound
 *
 */
Any_t Vector_at(Vector_t vec, size_t index);

/*
 * Function: Vector_remove(Vector_t vec, size_t index) -> Any_t
 *
 * Will remove the element at index `index` from vec and return it
 *
 * Parameters:
 *
 * - `Vector_t vec`: The vec to remove from
 * - `size_t index`: The index of the element to be removed
 *
 * Returns:
 *
 * - `On success`: the remove elem
 * - `On out of bound`: NULL
 *
 */
Any_t Vector_remove(Vector_t vec, size_t index);

/**
 * Function: Vector_iter(Vector_t vec, Any_t *elem) -> Boolean
 *
 * will iterate over vector `vec` elements and point the current element to
 * `elem` variable
 *
 * Parameters:
 *
 * - `Vector_t vec`: the vec to iterate
 * - `Any_t *elem`: The iterator var
 *
 * Returns:
 *
 * - 'True': if still elements to iterate
 * - `False`: if is not elements to iterate
 *
 */
Boolean Vector_iter(Vector_t vec, Any_t *elem);

/**
 * Function: Vector_iter_reset(Vector_t vec)
 *
 * Will reset the iterator to point back to the beginning of the vector
 *
 */
void Vector_iter_reset(Vector_t vec);




/*
 * Structure: Queue
 *
 * Represents a thread-safe queue data structure.
 *
 * Members:
 * - Node_t head: Pointer to the head of the queue.
 */
typedef struct {
  Node_t head;
  size_t size;
  Node_t tail;
  pthread_mutex_t mutx;
} Queue;

typedef Queue *Queue_t;

/*
 * Function: Queue_new() -> Queue_t
 *
 * Creates and initializes a new queue.
 *
 * Returns:
 * - Queue_t: Pointer to the newly created queue, or NULL if allocation fails.
 */
Queue_t Queue_new();

/*
 * Function: Queue_is_empty(Queue_t queue) -> int
 *
 * Checks if the queue is empty.
 *
 * Parameters:
 * - Queue_t queue: Pointer to the queue to check.
 *
 * Returns:
 * - `True`: if the queue is empty otherwise `False`
 */
Boolean Queue_is_empty(Queue_t queue);

/*
 * Function: Queue_dequeue(Queue_t queue) -> void*
 *
 * Removes and returns the data from the front of the queue.
 *
 * Parameters:
 * - Queue_t queue: Pointer to the queue from which to dequeue.
 *
 * Returns:
 * - void*: Pointer to the data removed from the queue, or NULL if the queue is
 * empty.
 */
void *Queue_dequeue(Queue_t queue);

/*
 * Function: Queue_enqueue(Queue_t queue, void *data) -> int
 *
 * Enqueues data at the end of the queue.
 *
 * Parameters:
 * - Queue_t queue: Pointer to the queue to which data is to be enqueued.
 * - void *data: Pointer to the data to be enqueued.
 *
 * Returns:
 * - int: 0 on success, -1 if allocation fails when creating a new node.
 */
int Queue_enqueue(Queue_t queue, Any_t data);

/*
 * Function: Queue_size(Queue_t queue) -> size_t
 *
 * Will return the number of elements in the Queue `queue`
 *
 * Parameters:
 *
 * - Queue_t queue: The queue to know its size
 *
 * Returns:
 *
 * - size_t: The number of elements in the Queue `queue`
 *
 */
size_t Queue_size(Queue_t queue);




/*
 * Typedef: Channel
 *
 * Represents a Multiple Producer Single Consumer thread-safe channel, where
 * many threads can put messages but only one thread should read the messages.
 *
 */
typedef struct {
  Queue_t queue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Channel;

/*
 * Typedef: Channel_t
 *
 * Is just a wrapper of `Channel *`
 *
 */
typedef Channel *Channel_t;

/*
 * Function: Channel_new() -> Channel_t
 *
 * Will create a new instance of `Channel` and returns it's pointer
 *
 * Returns:
 *
 * - Channel_t: a pointer to the created channel
 *
 */
Channel_t Channel_new();

/*
 * Function: Channel_send(Channel_t chan, Any_t message) -> int
 *
 * Will put the message `message` on the Channel `chan` and in case of error
 * will be printed in the stdout
 *
 * Parameters:
 *
 * - Channel_t chan: The channel where to put the message `message`
 * - Any_t message: The message to bet put on the Channel `chan`
 *
 * Returns:
 *
 * - 0: On success
 * - -1: On error
 *
 */
int Channel_send(Channel_t chan, Any_t message);

/*
 * Function: Channel_recv(Channel_t chan) -> Any_t
 *
 * Will read a message from the channel `chan`. If the channel is empty, the
 * current thread will blocked until some message be avalible
 *
 * `Note`: The read message will be popped out from the channel, thus, each
 * channel should have a Single Consumer to avoid loss of information
 *
 *
 * Parameters:
 *
 * - Channel_t chan: The channel where the message will be read
 *
 * Returns:
 *
 * - Any_t: a message read from channel `chan`
 *
 */
Any_t Channel_recv(Channel_t chan);




/*
 * Typedef: Stack
 *
 * Represents the stack LIFO data structure
 *
 */
typedef struct {
  Node_t head;
  size_t size;
  size_t max_stack;
  pthread_mutex_t mutx;
} Stack;

/*
 * Typedef: Stack_t
 *
 * It's just a type alias of `Stack *`
 *
 */
typedef Stack *Stack_t;

/*
 * Function: Stack_new(size_t max_stack) -> Stack_t
 *
 * Will create a new instance of Stack_t
 *
 * Parameters:
 *
 * - `size_t max_stack`: The max number of elements a stack can old at once,
 * provide `-1` to have a limite less stack
 *
 * Returns:
 *
 / - `Stack_t`: The new instance of Stack
 *
 */
Stack_t Stack_new(size_t max_stack);

/*
 * Function: Stack_is_empty(Stack_t stack) -> Boolean
 *
 * Will check if the stack is empty or not
 *
 * Parameters:
 *
 * - `Stack_t stack`: The stack to know if its empty
 *
 * Returns:
 *
 * - `True`: if the stack `stack` is empty
 * - `False`: if the stack `stack` is not empty
 *
 */
Boolean Stack_is_empty(Stack_t stack);

/*
 * Function: Stack_is_full(Stack_t stack) -> Boolean
 *
 * Will check if the Stack `stack` is full or not
 *
 * Parameters:
 *
 * - `Stack_t stack`: The stack to check weather is full
 *
 * Returns:
 *
 * - `True`: if the the count of elements in the stack is equal to stack limit
 * - `False`: if the the countog elements in the stack is less than stack limit
 *
 */
Boolean Stack_is_full(Stack_t stack);

/*
 * Function: Stack_push(Stack_t stack, Any_t data)
 *
 * Will put the value `data` on top of stack `stack`, if the stack `stack` has
 * limit and its full the value `data` won't be pushed and a StackOverFlow error
 * will trigged
 *
 * Parameters:
 *  - `Stack_t stack`: The stack to push onto
 *  - `Any_t data`: The value to be pushed
 *
 */
int Stack_push(Stack_t stack, Any_t data);

/*
 * Function: Stack_pop(Stack_t stack) -> Any_t
 *
 * Will remove and return the value on top of the stack `stack`, if the stack is
 * empty a StackUnderFlow will be trigged and the program will exit
 * unseccessfuly
 *
 * Parameters:
 *
 * - `Stack_t stack`: The stack to pop from it
 *
 * Returns:
 *
 * - will return the value on top of the stack
 *
 */
Any_t Stack_pop(Stack_t stack);




typedef void (*TestBody)();

struct Test {
  Str_t name;
  TestBody body;
  Boolean has_failed;
};

struct TestSuite {
  Str_t name;
  size_t cap;
  size_t count;
  size_t fails_count;
  struct Test **tests;
  struct Test *curr_test;
};

extern struct TestSuite *global_test_suite_object;

/*
 * INTERNAL API
 *
 */

void setup_global_test_suite_object(Str_t file);
struct Test *Test_new(Str_t name, TestBody body);
void register_test(Str_t name, TestBody body, Str_t filename);

/*
 * Public API
 *
 */

Boolean __assert_null__(Any_t expression, int line_number);
Boolean __assert_true__(Boolean expression, int line_number);
Boolean __assert_false__(Boolean expression, int line_number);
Boolean __assert_eq_int__(int expected, int provided, int line_number);
Boolean __assert_neq_int__(int expected, int provided, int line_number);
Boolean __assert_eq_str__(Str_t expected, Str_t provided, int line_number);
Boolean __assert_eq_struct__(Any_t expected, Any_t provided, int line_number);

/// Create a new test case
#define TEST(name)                                                             \
  static void test_##name();                                                   \
  static void __attribute__((constructor)) register_test_##name() {            \
    register_test(#name, test_##name, __FILE_NAME__);                          \
  }                                                                            \
  static void test_##name()

#define ASSERT_NULL(expression)                                                \
  if (!(__assert_null__((expression), __LINE__))) {                            \
    return;                                                                    \
  }

#define ASSERT_TRUE(expression)                                                \
  if (!(__assert_true__((expression), __LINE__))) {                            \
    return;                                                                    \
  }

#define ASSERT_FALSE(expression)                                               \
  if (!(__assert_false__((expression), __LINE__))) {                           \
    return;                                                                    \
  }

#define ASSERT_EQ_INT(expected, provided)                                      \
  if (!(__assert_eq_int__((expected), (provided), __LINE__))) {                \
    return;                                                                    \
  }

#define ASSERT_NEQ_INT(expected, provided)                                     \
  if (!(__assert_neq_int__((expected), (provided), __LINE__))) {               \
    return;                                                                    \
  }

#define ASSERT_EQ_STR(expected, provided)                                      \
  if (!(__assert_eq_str__((expected), (provided), __LINE__))) {                \
    return;                                                                    \
  }

#define ASSERT_EQ_STRUCT(expected, provided)                                   \
  if (!(__assert_eq_struct__((expected), (provided), __LINE__))) {             \
    return;                                                                    \
  }

/// Will run all `TEST()` cases and returns the fails count
size_t RUN_ALL_TESTS();




typedef enum {
  INFO = 0,
  WARN = 1,
  ERROR = 2,
} LogLevel;

/// Will convert a LogLevel enum entry to its respective string representation
#define LogLevel(level)                                                        \
  (level == INFO)    ? "\033[0;32mINFO\033[0m"                                 \
  : (level == WARN)  ? "\033[0;33mWARNING\033[0m"                              \
  : (level == ERROR) ? "\033[0;31mERROR\033[0m"                                \
                     : "UNKNOWN LOG LEVEL"
/**
 * Function: Log(FILE *file, LogLevel level, Str_t message, ...)
 *
 * will log a message to the specified file
 *
 */
void Log(FILE *file, LogLevel level, Str_t message, ...);

#endif // C_THINGS
