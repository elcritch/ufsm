#include <assert.h>
#include <stdio.h>
#include <test_xmi_machine_input.h>
#include <ufsm.h>
#include "common.h"

static bool flag_eC = false;
static bool flag_eD = false;
static bool flag_t1 = false;
static bool flag_t2 = false;
static bool flag_t3 = false;
static bool flag_final = false;

static void reset_flags(void)
{
    flag_eC = false;
    flag_eD = false;
    flag_t1 = false;
    flag_t2 = false;
    flag_t3 = false;
    flag_final = false;
}

bool Guard(ufsm_sm_t *sm, ufsm_guard_t *a) { return true; }

void DoAction(ufsm_sm_t *sm, ufsm_action_t *a) {}

void eD(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { flag_eD = true; }

void eC(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { flag_eC = true; }

void t1(ufsm_sm_t *sm, ufsm_action_t *a) { flag_t1 = true; }

void t2(ufsm_sm_t *sm, ufsm_action_t *a) { flag_t2 = true; }

void t3(ufsm_sm_t *sm, ufsm_action_t *a) { flag_t3 = true; }

void final(ufsm_sm_t *sm, ufsm_action_t *a) { flag_final = true; }

int main(void)
{
    struct ufsm_machine* m = get_StateMachine1();
    uint32_t err = UFSM_OK;
    test_init(m);

    assert(ufsm_init_machine(m) == UFSM_OK);
    assert(flag_eC);

    reset_flags();
    test_process(m, EV_D);
    test_process(m, EV_B);

    test_process(m, EV_E);
    test_process(m, EV_B);
    test_process(m, EV_A);
    assert(flag_eD);

    reset_flags();
    test_process(m, EV_B);
    test_process(m, EV_E);
    assert(!flag_t1 && !flag_t2 && !flag_t3);
    err = ufsm_process(m, EV_E1);
    if (err != UFSM_OK)
    {
        printf("Error: %i\n", err);
        assert(0);
    }

    assert(flag_t1);
    assert(!flag_t2);
    assert(!flag_t3);
    assert(flag_t1 && !flag_t2 && !flag_t3);
    assert(ufsm_process(m, EV_E2) == UFSM_OK);
    assert(flag_t1 && flag_t2 && !flag_t3);
    assert(ufsm_process(m, EV_E3) == UFSM_OK);
    assert(flag_t1 && flag_t2 && flag_t3);
    assert(flag_final);
    return 0;
}
