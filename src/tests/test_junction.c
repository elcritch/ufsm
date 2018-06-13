#include <stdio.h>
#include <assert.h>
#include <ufsm.h>
#include <test_junction_input.h>
#include "common.h"

static bool flag_eA = false;
static bool flag_eB = false;
static bool flag_eC = false;
static bool flag_eD = false;
static bool flag_t1 = false;

static void reset_flags()
{
    flag_eA = false;
    flag_eB = false;
    flag_eC = false;
    flag_eD = false;
    flag_t1 = false;
}

void eA(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eA = true;
}

void eB(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eB = true;
}

void eC(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eC = true;
}

void eD(ufsm_machine_t *m, ufsm_state_t *t)
{
    flag_eD = true;
}

void t1(ufsm_machine_t *m, ufsm_trans_t *t)
{
    flag_t1 = true;
}


int main(int argc, char **argv) 
{
    struct ufsm_machine *m = get_StateMachine1();
    
    test_init(m);
    ufsm_init_machine(m);
    
    assert (flag_eA &&
            !flag_eB &&
            !flag_eC &&
            !flag_eD &&
            !flag_t1);

    reset_flags();
    test_process(m, EV_J);
    
    assert (!flag_eA &&
            !flag_eB &&
            !flag_eC &&
            flag_eD &&
            flag_t1);

    ufsm_reset_machine(m);
    ufsm_init_machine(m);
    reset_flags();

    test_process(m, EV);
    test_process(m, EV);
    test_process(m, EV_J);

    assert (!flag_eA &&
            flag_eB &&
            flag_eC &&
            flag_eD &&
            flag_t1);



    return 0;
}
