#include <assert.h>
#include <stdio.h>
#include <test_choice2_input.h>
#include <ufsm.h>
#include "common.h"

static bool flag_e2 = false;
static bool flag_t1 = false;
static bool flag_t2 = false;
static bool g_val = true;

void e2(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { flag_e2 = true; }

bool g(ufsm_sm_t *sm, ufsm_guard_t *a) { return g_val; }

void t1(ufsm_sm_t *sm, ufsm_action_t *a) { flag_t1 = true; }

void t2(ufsm_sm_t *sm, ufsm_action_t *a) { flag_t2 = true; }

int main(void)
{
    struct ufsm_machine* m = get_StateMachine1();

    test_init(m);
    ufsm_init_machine(m);

    test_process(m, EV);

    assert(!flag_e2 && flag_t1 && flag_t2);

    ufsm_reset_machine(m);
    flag_e2 = false;
    flag_t1 = false;
    flag_t2 = false;
    g_val = false;
    ufsm_init_machine(m);

    test_process(m, EV);

    assert(flag_e2 && !flag_t1 && !flag_t2);

    return 0;
}
