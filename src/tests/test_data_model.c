#include <assert.h>
#include <stdio.h>

/* #define UFSM_META_TYPE te_expr */

#include <ufsm.h>
#include "common.h"
#include "tinyexpr.h"
/* #include "tinystd.h" */

enum events
{
    EV_Z,
    EV_A,
    EV_B
};

struct data_model
{
  int count;
  int _event_data;
};

struct data_model *data_model;

static struct ufsm_region region1;

static struct ufsm_machine m = {
  .name = "Simple Test Machine",
  .region = &region1,
};

static bool flag_guard1_called = false;
static bool flag_action1_called = false;
static bool guard1_ret_val = true;

/* static int data_count = 5; */

static void reset_test_flags(void)
{
    flag_guard1_called = false;
    flag_action1_called = false;
    guard1_ret_val = true;
}

static bool guard_func(ufsm_sm_t *sm, ufsm_guard_t *g)
{
    printf("guard function: (count = %d)\n", data_model->count);
    /* flag_guard1_called = true; */
    te_print(g->meta);

    return (bool) te_eval(g->meta);
}

static void action_func(ufsm_sm_t *sm, ufsm_action_t *a)
{
  printf("action func: \n");
  /* flag_action1_called = true; */
  /* count--; */

  if (sm->_event->data)
    data_model->_event_data = *((int*)sm->_event->data);
  else
    data_model->_event_data = 0;

  if (a->meta != NULL) {
    te_print(a->meta);

    te_eval(a->meta);
  } else {
    assert(false);
  }

  struct ufsm_queue* q = ufsm_get_queue(&m);
  ufsm_queue_put(q, EV_A);
}

static struct ufsm_state A;
static struct ufsm_region region1;

static struct ufsm_state simple_INIT = {
    .name = "Init",
    .kind = UFSM_STATE_INIT,
    .parent_region = &region1,
    .next = &A
};

static struct ufsm_state B = {
    .name = "State B",
    .kind = UFSM_STATE_SIMPLE,
    .parent_region = &region1,
    .next = NULL,
};

static struct ufsm_state A = {
    .name = "State A",
    .kind = UFSM_STATE_SIMPLE,
    .parent_region = &region1,
    .next = &B,
};

static struct ufsm_guard guard1 = {
    .f = &guard_func,
    .id = "count > 0",
    .meta = NULL,
    .next = NULL,
};

static struct ufsm_action action1 = {
    .f = &action_func,
    .id = "count = count - 1",
    .meta = NULL,
    .next = NULL,
};

static struct ufsm_action action_init = {
  .f = &action_func,
  .id = "count = _event.data",
  .meta = NULL,
  .next = NULL,
};

static struct ufsm_transition simple_transition_B = {
    .name = "EV_B",
    .trigger = EV_B,
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &A,
    .action = &action_init,
    .dest = &B,
    .next = NULL
};

static struct ufsm_transition simple_transition_A = {
  .name = "EV_A",
  .trigger = EV_A,
  .kind = UFSM_TRANSITION_EXTERNAL,
  .source = &B,
  .dest = &A,
  .next = &simple_transition_B
};

static struct ufsm_transition simple_transition_B_B_repeat = {
  .name = "EV_A",
  .trigger = EV_A,
  .kind = UFSM_TRANSITION_EXTERNAL,
  .source = &B,
  .dest = &B,
  .guard = &guard1,
  .action = &action1,
  .next = &simple_transition_A
};

static struct ufsm_transition simple_transition_INIT = {
    .name = "Init",
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &simple_INIT,
    .trigger = UFSM_NO_TRIGGER,
    .dest = &A,
    .next = &simple_transition_B_B_repeat ,
};

static struct ufsm_region region1 = {.state = &simple_INIT,
                                     .transition = &simple_transition_INIT,
                                     .next = NULL};

void print_state(void *ptr)
{
  if (ptr == &A)
    printf(" - state A\n");
  else if (ptr == &B)
    printf(" - state B\n");
}

int main(void)
{
    // Init Data Model

    int guard1_err = 0;
    int action1_err = 0;

    printf("guard mesg: %s : %p\n", guard1.meta, guard1.meta);

    struct data_model data = {.count = 0};
    data_model = &data;

    te_variable vars[] = {
      {"count", &data.count, 0, 0},
      {"_event.data", &data._event_data, 0, 0},
      {NULL, NULL, 0, 0},
    };

    // Guard
    guard1.meta = te_compile(guard1.id, vars, 2, &guard1_err);
    printf("guard expr:\n");
    te_print(guard1.meta);

    if (guard1_err)
      printf("guard parse error: `%s` : %d\n", guard1.id, guard1_err);
    assert(guard1_err == 0 && "Initializing guard expr");

    // Action1
    action1.meta = te_compile(action1.id, vars, 2, &action1_err);
    printf("action expr:\n");
    te_print(action1.meta);

    if (action1_err)
      printf("action parse error: `%s` : %d\n", action1.id, action1_err);
    assert(action1_err == 0 && "Initializing action expr");

    // Action Init
    action_init.meta = te_compile(action_init.id, vars, 2, &action1_err);

    if (action1_err)
      printf("action parse error: `%s` : %d\n", action_init.id, action1_err);

    assert(action1_err == 0 && "Initializing action expr");
    printf("action init expr:\n");
    te_print(action_init.meta);


    // Init Test
    ufsm_status_t err;
    reset_test_flags();
    test_init(&m);

    printf(" - status UFSM_OK: %d\n", UFSM_OK);
    printf(" - status UFSM_OK: %d\n", UFSM_ERROR);
    printf("\n");

    // Init SM
    err = ufsm_init_machine(&m);
    assert(err == UFSM_OK && "Initializing statemachine");
    assert(m.region->current == &A);

    /* assert(flag_guard1_called == false); */
    /* assert(flag_action1_called == false); */

    // Trans from A -> B
    /* reset_test_flags(); */
    printf("1. A -> B \n");
    int count_init = 5;
    struct ufsm_event item = {EV_B, &count_init};
    err = ufsm_process_item(&m, item);

    assert(m.region->current == &B && err == UFSM_OK);

    // Trans from B -> A
    reset_test_flags();
    printf("2. B -> A \n");
    err = ufsm_process(&m, EV_A);
    printf(" - state %p\n", m.region->current);
    assert(m.region->current == &B && err == UFSM_OK);

    int limit = 0;
    int test_count = 0;

    // TODO: ufsm_process_done() function (?)
    while ((err = ufsm_process_queue(&m)) == UFSM_OK && limit++ < 20)
    {
        test_count++;
        printf("2.1 B -> A \n");
        print_state(m.region->current);
    }

    assert(test_count == count_init);
    assert(limit < 20);
    assert(m.region->current == &A);

    /* assert(flag_guard1_called); */
    /* assert(flag_action1_called); */

    return 0;
}
