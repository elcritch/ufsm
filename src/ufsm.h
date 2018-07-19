/**
 * uFSM
 *
 * Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef UFSM_H
#define UFSM_H

#include <stdbool.h>
#include <stdint.h>
#include "tinyexpr.h"

/* Error codes */

enum ufsm_status_codes
{
    UFSM_OK,
    UFSM_ERROR,
    UFSM_ERROR_NO_INIT_REGION,
    UFSM_ERROR_UNKNOWN_STATE_KIND,
    UFSM_ERROR_EVENT_NOT_PROCESSED,
    UFSM_ERROR_LCA_NOT_FOUND,
    UFSM_ERROR_STACK_OVERFLOW,
    UFSM_ERROR_STACK_UNDERFLOW,
    UFSM_ERROR_QUEUE_EMPTY,
    UFSM_ERROR_QUEUE_FULL,
    UFSM_ERROR_MACHINE_TERMINATED,
};

typedef enum ufsm_status_codes ufsm_status_t;
typedef int32_t event_t;

extern const char* ufsm_errors[];

/* Misc defines */
#define UFSM_NO_TRIGGER -1
#define UFSM_COMPLETION_EVENT -1

#ifndef UFSM_STACK_SIZE
#define UFSM_STACK_SIZE 128
#endif

#ifndef UFSM_COMPLETION_STACK_SIZE
#define UFSM_COMPLETION_STACK_SIZE 16
#endif

#ifndef UFSM_QUEUE_SIZE
#define UFSM_QUEUE_SIZE 16
#endif

#ifndef UFSM_DEFER_QUEUE_SIZE
#define UFSM_DEFER_QUEUE_SIZE 16
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef UFSM_META_TYPE
#define UFSM_META_TYPE void
#endif

#ifdef UFSM_DEBUG
#define DEBUG(SM, DEBUG, VAR) \
  if (SM->debug.DEBUG) {SM->debug.DEBUG(VAR);}

#define DEBUG2(SM, DEBUG, VAR1, VAR2) \
  if (SM->debug.DEBUG) {SM->debug.DEBUG(VAR1, VAR2);}

#else

#define DEBUG(SM, DEBUG, VAR) {}
#define DEBUG2(SM, DEBUG, VAR1, VAR2) {}

#endif

typedef UFSM_META_TYPE ufsm_meta_t;

struct ufsm_event;
struct ufsm_state;
struct ufsm_machine;
struct ufsm_action;
struct ufsm_guard;
struct ufsm_transition;
struct ufsm_region;
struct ufsm_entry_exit;
struct ufsm_doact;

typedef struct ufsm_machine ufsm_sm_t;
typedef struct ufsm_state ufsm_state_t;
typedef struct ufsm_guard ufsm_guard_t;
typedef struct ufsm_action ufsm_action_t;
typedef struct ufsm_transition ufsm_trans_t;
typedef struct ufsm_region ufsm_region_t;
typedef struct ufsm_entry_exit ufsm_entry_exit_t;
typedef struct ufsm_doact ufsm_doact_t;

typedef bool (*ufsm_guard_func_t)(ufsm_sm_t *m, ufsm_guard_t *g);
typedef void (*ufsm_action_func_t)(ufsm_sm_t *m, ufsm_action_t *g);
typedef void (*ufsm_entry_exit_func_t)(ufsm_sm_t *m, ufsm_entry_exit_t *e);

typedef void (*ufsm_queue_cb_t)(void);
typedef uint32_t (*ufsm_doact_cb_t)(ufsm_sm_t* m,
                                    ufsm_state_t* s);

typedef void (*ufsm_dostop_func_t)(ufsm_sm_t* m,
                                   ufsm_state_t* s,
                                   ufsm_doact_t* t);

typedef void (*ufsm_doact_func_t)(ufsm_sm_t* m,
                                  ufsm_state_t* s,
                                  ufsm_doact_cb_t cb);

/* Debug callbacks */
typedef void (*ufsm_debug_event_t)(event_t ev);
typedef void (*ufsm_debug_transition_t)(struct ufsm_transition* t);
typedef void (*ufsm_debug_enter_region_t)(struct ufsm_region* region);
typedef void (*ufsm_debug_leave_region_t)(struct ufsm_region* region);
typedef void (*ufsm_debug_guard_t)(struct ufsm_guard* guard, bool result);
typedef void (*ufsm_debug_action_t)(struct ufsm_action* action);
typedef void (*ufsm_debug_enter_state_t)(struct ufsm_state* s);
typedef void (*ufsm_debug_exit_state_t)(struct ufsm_state* s);
typedef void (*ufsm_debug_reset_t)(struct ufsm_machine* m);

enum ufsm_transition_kind
{
    UFSM_TRANSITION_EXTERNAL,
    UFSM_TRANSITION_INTERNAL,
    UFSM_TRANSITION_LOCAL,
};

extern const char* ufsm_errors[];

enum ufsm_state_kind
{
    UFSM_STATE_SIMPLE,
    UFSM_STATE_INIT,
    UFSM_STATE_FINAL,
    UFSM_STATE_SHALLOW_HISTORY,
    UFSM_STATE_DEEP_HISTORY,
    UFSM_STATE_EXIT_POINT,
    UFSM_STATE_ENTRY_POINT,
    UFSM_STATE_JOIN,
    UFSM_STATE_FORK,
    UFSM_STATE_CHOICE,
    UFSM_STATE_JUNCTION,
    UFSM_STATE_TERMINATE,
};

extern const char* ufsm_state_kinds[];

struct ufsm_stack
{
    uint32_t no_of_elements;
    void** data;
    uint32_t pos;
};

struct ufsm_event {
    event_t ev;
    void *data;
};

typedef struct ufsm_event ufsm_event_item_t;

struct ufsm_queue {
    uint32_t no_of_elements;
    uint32_t s;
    uint32_t head;
    uint32_t tail;
    struct ufsm_event *data;
    ufsm_queue_cb_t on_data;
    ufsm_queue_cb_t lock;
    ufsm_queue_cb_t unlock;
};

struct ufsm_machine_debug
{
  ufsm_debug_event_t event;
  ufsm_debug_transition_t transition;
  ufsm_debug_enter_region_t enter_region;
  ufsm_debug_leave_region_t leave_region;
  ufsm_debug_guard_t guard;
  ufsm_debug_action_t action;
  ufsm_debug_enter_state_t enter_state;
  ufsm_debug_exit_state_t exit_state;
  ufsm_debug_reset_t reset;
};

struct ufsm_machine
{
    const char* id;
    const char* name;

#ifdef UFSM_DEBUG
    struct ufsm_machine_debug debug;
#endif

    bool terminated;
    struct ufsm_event *_event;

    void *data_model;
    void *stack_data[UFSM_STACK_SIZE];
    void *completion_stack_data[UFSM_COMPLETION_STACK_SIZE];
    struct ufsm_event queue_data[UFSM_QUEUE_SIZE];
    struct ufsm_event defer_queue_data[UFSM_DEFER_QUEUE_SIZE];

    struct ufsm_queue queue;
    struct ufsm_queue defer_queue;

    struct ufsm_state* parent_state;
    struct ufsm_stack stack;
    struct ufsm_stack completion_stack;

    struct ufsm_region* region;
    struct ufsm_machine* next;
};

struct ufsm_action
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    ufsm_action_func_t f;
    struct ufsm_action* next;
};

struct ufsm_guard
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    ufsm_guard_func_t f;
    struct ufsm_guard* next;
};

struct ufsm_entry_exit
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    ufsm_entry_exit_func_t f;
    struct ufsm_entry_exit* next;
};

struct ufsm_doact
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    ufsm_doact_func_t f_start;
    ufsm_dostop_func_t f_stop;
    struct ufsm_doact* next;
};

struct ufsm_transition
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    const char* trigger_name;
    int32_t trigger;
    bool defer;
    enum ufsm_transition_kind kind;
    struct ufsm_action* action;
    struct ufsm_guard* guard;
    struct ufsm_state* source;
    struct ufsm_state* dest;
    struct ufsm_transition* next;
};

struct ufsm_region
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    bool has_history;
    struct ufsm_state* current;
    struct ufsm_state* history;
    struct ufsm_state* state;
    struct ufsm_transition* transition;
    struct ufsm_state* parent_state;
    struct ufsm_region* next;
};

struct ufsm_state
{
    const char* id;
    const char* name;
    ufsm_meta_t *meta;
    enum ufsm_state_kind kind;
    struct ufsm_entry_exit* entry;
    struct ufsm_doact* doact;
    struct ufsm_entry_exit* exit;
    struct ufsm_region* region;
    struct ufsm_region* parent_region;
    struct ufsm_machine* submachine;
    struct ufsm_state* next;
};

ufsm_status_t ufsm_init_machine(struct ufsm_machine *m);
ufsm_status_t ufsm_reset_machine(struct ufsm_machine *m);
ufsm_status_t ufsm_process_item(struct ufsm_machine *m,
                                struct ufsm_event item);
ufsm_status_t ufsm_process(struct ufsm_machine *m, event_t ev);
ufsm_status_t ufsm_process_queue(struct ufsm_machine* m);
ufsm_status_t ufsm_stack_init(struct ufsm_stack *stack,
                            uint32_t no_of_elements,
                            void **stack_data);
ufsm_status_t ufsm_stack_push(struct ufsm_stack *stack, void *item);
ufsm_status_t ufsm_stack_pop(struct ufsm_stack *stack, void **item);

ufsm_status_t ufsm_queue_init(struct ufsm_queue *q, uint32_t no_of_elements,
                                            struct ufsm_event *data);

ufsm_status_t ufsm_queue_put(struct ufsm_queue *q, event_t ev);
ufsm_status_t ufsm_queue_get(struct ufsm_queue *q, event_t *ev);

ufsm_status_t ufsm_queue_put_item(struct ufsm_queue *q, ufsm_event_item_t item);
ufsm_status_t ufsm_queue_get_item(struct ufsm_queue *q, ufsm_event_item_t *item);

struct ufsm_queue * ufsm_get_queue(struct ufsm_machine *m);

#endif
