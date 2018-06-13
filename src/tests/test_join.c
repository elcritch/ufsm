#include <stdio.h>
#include <assert.h>
#include <ufsm.h>
#include <test_join_input.h>
#include "common.h"

static bool flag_final = false;

void final(ufsm_machine_t *m, ufsm_transition_t *t)
{
    flag_final = true;
}

void eAB(ufsm_machine_t *m, ufsm_state_t *t)
{
}

void xAB(ufsm_machine_t *m, ufsm_state_t *t)
{
}

int main(int argc, char **argv) 
{
    struct ufsm_machine *m = get_StateMachine1();
    
    test_init(m);
    ufsm_init_machine(m);

    test_process(m, EV);

    test_process(m, EV);



    assert (flag_final);
    return 0;
}
