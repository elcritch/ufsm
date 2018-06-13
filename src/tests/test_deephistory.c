#include <stdio.h>
#include <assert.h>
#include <ufsm.h>
#include <test_deephistory_input.h>
#include "common.h"

static bool flag_final = false;
static bool flag_eB = false;
static bool flag_eA2 = false;
static bool flag_xA2 = false;
static bool flag_eA1 = false;
static bool flag_xA1 = false;
static bool flag_eE = false;
static bool flag_xE = false;
static bool flag_eD = false;
static bool flag_xD = false;
static bool flag_eC = false;
static bool flag_xC = false;
static bool flag_eA = false;
static bool flag_xA = false;

static void reset_flags()
{
    flag_final = false;
    flag_eB = false;
    flag_eA2 = false;
    flag_xA2 = false;
    flag_eA1 = false;
    flag_xA1 = false;
    flag_eE = false;
    flag_xE = false;
    flag_eD = false;
    flag_xD = false;
    flag_eC = false;
    flag_xC = false;
    flag_eA = false;
    flag_xA = false;
}

void final(ufsm_machine_t *m, ufsm_transition_t *t)
{
    flag_final = true;
}

void eB(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eB = true;
}

void eA2(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eA2 = true;
}

void xA2(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xA2 = true;
}

void eA1(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eA1 = true;
}

void xA1(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xA1 = true;
}

void eE(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eE = true;
}

void xE(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xE = true;
}

void eD(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eD = true;
}

void xD(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xD = true;
}

void eC(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eC = true;
}

void xC(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xC = true;
}

void eA(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eA = true;
}

void xA(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_xA = true;
}

int main(int argc, char **argv) 
{
    struct ufsm_machine *m = get_StateMachine1();
    
    test_init(m);
    ufsm_init_machine(m);
    assert(!flag_final &&
        flag_eB &&
        !flag_eA2 &&
        !flag_xA2 &&
        !flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        !flag_eD &&
        !flag_xD &&
        !flag_eC &&
        !flag_xC &&
        !flag_eA &&
        !flag_xA);

    reset_flags();
    test_process(m, EV_A);
    assert(!flag_final &&
        !flag_eB &&
        !flag_eA2 &&
        !flag_xA2 &&
        flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        !flag_eD &&
        !flag_xD &&
        !flag_eC &&
        !flag_xC &&
        flag_eA &&
        !flag_xA);


    test_process(m, EV_1);
    test_process(m, EV_1);
    test_process(m, EV_1);

    assert(!flag_final &&
        !flag_eB &&
        flag_eA2 &&
        flag_xA2 &&
        flag_eA1 &&
        flag_xA1 &&
        flag_eE &&
        flag_xE &&
        flag_eD &&
        !flag_xD &&
        flag_eC &&
        !flag_xC &&
        flag_eA &&
        !flag_xA);

    reset_flags();
    test_process(m, EV_B);


    assert(!flag_final &&
        flag_eB &&
        !flag_eA2 &&
        !flag_xA2 &&
        !flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        !flag_eD &&
        flag_xD &&
        !flag_eC &&
        flag_xC &&
        !flag_eA &&
        flag_xA);

    reset_flags();
    test_process(m, EV_A);


    assert(!flag_final &&
        !flag_eB &&
        !flag_eA2 &&
        !flag_xA2 &&
        !flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        flag_eD &&
        !flag_xD &&
        flag_eC &&
        !flag_xC &&
        flag_eA &&
        !flag_xA);

    reset_flags();
    test_process(m, EV_1);

    assert(!flag_final &&
        !flag_eB &&
        !flag_eA2 &&
        !flag_xA2 &&
        flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        !flag_eD &&
        flag_xD &&
        !flag_eC &&
        flag_xC &&
        !flag_eA &&
        !flag_xA);
    
    test_process(m ,EV_1);
    reset_flags();
    test_process(m, EV_EXIT);

    assert(flag_final &&
        !flag_eB &&
        !flag_eA2 &&
        flag_xA2 &&
        !flag_eA1 &&
        !flag_xA1 &&
        !flag_eE &&
        !flag_xE &&
        !flag_eD &&
        !flag_xD &&
        !flag_eC &&
        !flag_xC &&
        !flag_eA &&
        flag_xA);


    return 0;
}
