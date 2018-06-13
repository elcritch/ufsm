#include <stdio.h>
#include <ufsm.h>
#include <assert.h>
#include <test_compound_transition_input.h>
#include "common.h"

static bool flag_eT1 = false;
static bool flag_eT11 = false;
static bool flag_eT111 = false;
static bool flag_xS1 = false;
static bool flag_xS11 = false;
static bool flag_t1 = false;
static bool flag_t2 = false;
static bool flag_t3 = false;

static void reset_flags(void)
{
    flag_eT1 = false;
    flag_eT11 = false;
    flag_eT111 = false;
    flag_xS1 = false;
    flag_xS11 = false;
    flag_t1 = false;
    flag_t2 = false;
    flag_t3 = false;
}

void t1(void *m, ufsm_trans_t *t) 
{
    flag_t1 = true;
    assert (flag_xS11 && !flag_eT1 && !flag_eT11 && !flag_eT111 &&
                !flag_xS1 && flag_t1 && !flag_t2 && !flag_t3);
}
void t2(void *m, ufsm_trans_t *t) 
{
    flag_t2 = true;
    assert (flag_xS11 && !flag_eT1 && !flag_eT11 && !flag_eT111 &&
                flag_xS1 && flag_t1 && flag_t2 && !flag_t3);
}

void t3(void *m, ufsm_trans_t *t) 
{
    flag_t3 = true;
    assert (flag_xS11 && flag_eT1 && flag_eT11 && !flag_eT111 &&
                flag_xS1 && flag_t1 && flag_t2 && flag_t3);
}

void eT1(void *m, ufsm_state_t *t)
{
    assert (flag_xS1 && flag_xS11);
    flag_eT1 = true;
}

void eT11(void *m, ufsm_state_t *t)
{
    assert (flag_eT1);
    flag_eT11 = true;
}

void eT111(void *m, ufsm_state_t *t)
{
    assert (flag_eT1 && flag_eT11);
    flag_eT111 = true;
}

void xS1(void *m, ufsm_state_t *t)
{
    assert (flag_xS11);
    flag_xS1 = true;
}

void xS11(void *m, ufsm_state_t *t)
{
 
    assert(!flag_eT1 && !flag_eT11 && !flag_eT111);
    assert(!flag_xS1 && !flag_xS11);


    flag_xS11 = true;
}

int main(int argc, char **argv) 
{
    void *m = get_StateMachine1();
 
    test_init(m);
    reset_flags();
    ufsm_init_machine(m);
    
    assert(!flag_eT1 && !flag_eT11 && !flag_eT111);
    assert(!flag_xS1 && !flag_xS11);

    reset_flags();
    ufsm_process(m, sig);       
    assert(flag_xS1 && flag_xS11);
    assert(flag_eT1 && flag_eT11 && flag_eT111);
 

    return 0;
}
