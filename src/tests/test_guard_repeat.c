#include <assert.h>
#include <stdio.h>
#include <ufsm.h>
#include "common.h"

enum events
{
    EV_A,
    EV_B
};

static struct ufsm_region region1;

static struct ufsm_machine m = {
  .name = "Simple Test Machine",
  .region = &region1,
};

static bool flag_guard1_called = false;
static bool flag_action1_called = false;
static bool guard1_ret_val = true;

static int data_count = 5;

static void reset_test_flags(void)
{
    flag_guard1_called = false;
    flag_action1_called = false;
    guard1_ret_val = true;
}

static bool guard1_f(void)
{
    printf("guard1\n");
    flag_guard1_called = true;
    return data_count > 0;
}

static void action1_f(void)
{
  printf("action1\n");
  flag_action1_called = true;
  data_count--;

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
    .f = &guard1_f,
    .next = NULL,
};

static struct ufsm_action action1 = {
    .f = &action1_f,
    .next = NULL,
};

static struct ufsm_transition simple_transition_B = {
    .name = "EV_B",
    .trigger = EV_B,
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &A,
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

int main(void)
{
    ufsm_status_t err;

    reset_test_flags();
    test_init(&m);

    printf("states:\n");
    printf(" - state A: %d\n", &A);
    printf(" - state B: %d\n", &B);
    printf("\n");

    // Init SM
    err = ufsm_init_machine(&m);
    assert(err == UFSM_OK && "Initializing");
    assert(m.region->current == &A);
    /* assert(flag_guard1_called == false); */
    /* assert(flag_action1_called == false); */

    // Trans from A -> B
    /* reset_test_flags(); */
    printf("1. A -> B \n");
    err = ufsm_process(&m, EV_B);
    assert(m.region->current == &B && err == UFSM_OK);

    // Trans from B -> A
    reset_test_flags();
    printf("2. B -> A \n");
    err = ufsm_process(&m, EV_A);
    printf(" - state %d\n", m.region->current);
    assert(m.region->current == &B && err == UFSM_OK);

    int limit = 0;
    event_t ev = -1;
    ufsm_status_t qerr;

    // TODO: ufsm_process_done() function (?)
    while (true && limit++ < 100 )
    {
        printf("2.1 B -> A \n");
        qerr = ufsm_queue_get(&m.queue, &ev);
        if (qerr == UFSM_OK)
        {
          assert(m.region->current == &B && err == UFSM_OK);
          err = ufsm_process(&m, ev);
          printf(" - state %d\n", m.region->current);
        }
        else
        {
          break;
        }
    }

    assert(limit < 100);
    assert(m.region->current == &A && err == UFSM_OK);

    /* assert(flag_guard1_called); */
    /* assert(flag_action1_called); */

    return 0;
}
