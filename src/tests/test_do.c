#include <assert.h>
#include <stdio.h>
#include <test_do_input.h>
#include <ufsm.h>
#include "common.h"

static bool flag_final = false;
static bool call_cb = true;
static bool flag_dA_stop = false;

void xA(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { assert(flag_dA_stop); }

void eA(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { assert(!flag_dA_stop); }

void dA_start(struct ufsm_machine* m, struct ufsm_state* s, ufsm_doact_cb_t cb)
{
    if (call_cb)
        cb(m, s);
}

void dA_stop(ufsm_sm_t *sm, ufsm_state_t *s, ufsm_doact_t *a) { flag_dA_stop = true; }

void final(ufsm_sm_t *sm, ufsm_action_t *a) { flag_final = true; }

int main(void)
{
    struct ufsm_machine* m = get_StateMachine1();

    test_init(m);
    ufsm_init_machine(m);

    assert(flag_final && flag_dA_stop);

    flag_final = false;
    call_cb = false;
    flag_dA_stop = false;
    ufsm_reset_machine(m);
    ufsm_init_machine(m);

    test_process(m, EV);

    assert(!flag_final && flag_dA_stop);
    return 0;
}
