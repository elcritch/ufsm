#include <stdio.h>
#include <assert.h>
#include <ufsm.h>
#include <test_fork_input.h>
#include "common.h"

static bool gA_val = false;
static bool g2_val = false;
static bool flag_gA = false;
static bool flag_g2 = false;
static bool flag_finalD = false;
static bool flag_eD = false;
static bool flag_eC = false;
static bool flag_finalC = false;
static bool flag_eB2 = false;
static bool flag_xB2 = false;
static bool flag_eA2 = false;
static bool flag_xA2 = false;
static bool flag_eAB = false;
static bool flag_xAB = false;
static uint32_t ab_exit_cnt = 0;

void reset_flags()
{
    flag_gA = false;
    flag_g2 = false;
    flag_finalD = false;
    flag_eD = false;
    flag_eC = false;
    flag_finalC = false;
    flag_eB2 = false;
    flag_xB2 = false;
    flag_eA2 = false;
    flag_xA2 = false;
    flag_eAB = false;
    flag_xAB = false;
}

bool gA(ufsm_machine_t *m, ufsm_trans_t *t) { flag_gA = true; return gA_val; }
bool g2(ufsm_machine_t *m, ufsm_trans_t *t) { flag_g2 = true; return g2_val; }
void finalD(ufsm_machine_t *m, ufsm_trans_t *t) { flag_finalD = true; }
void eD(ufsm_machine_t *m, ufsm_state_t *t) { flag_eD = true; }
void eC(ufsm_machine_t *m, ufsm_state_t *t) { flag_eC = true; }
void finalC(ufsm_machine_t *m, ufsm_trans_t *t) { flag_finalC = true; }
void eB2(ufsm_machine_t *m, ufsm_state_t *t) { flag_eB2 = true; }
void xB2(ufsm_machine_t *m, ufsm_state_t *t) { flag_xB2 = true; }
void eA2(ufsm_machine_t *m, ufsm_state_t *t) { flag_eA2 = true; }
void xA2(ufsm_machine_t *m, ufsm_state_t *t) { flag_xA2 = true; }
void eAB(ufsm_machine_t *m, ufsm_state_t *t) { flag_eAB = true; }
void xAB(ufsm_machine_t *m, ufsm_state_t *t) { ab_exit_cnt++; flag_xAB = true; }

int main(int argc, char **argv) 
{
    struct ufsm_machine *m = get_StateMachine1();
    
    test_init(m);
    ufsm_init_machine(m);

    assert (!flag_finalD &&
            !flag_gA &&
            !flag_g2 &&
            !flag_eD &&
            !flag_eC &&
            !flag_finalC &&
            !flag_eB2 &&
            !flag_xB2 &&
            !flag_eA2 &&
            !flag_xA2 &&
            !flag_eAB &&
            !flag_xAB);

    g2_val = true;
    gA_val = false;
    test_process(m, EV);

    assert (!flag_finalD &&
            flag_gA &&
            flag_g2 &&
            !flag_eD &&
            !flag_eC &&
            !flag_finalC &&
            flag_eB2 &&
            !flag_xB2 &&
            flag_eA2 &&
            !flag_xA2 &&
            flag_eAB &&
            !flag_xAB);

    reset_flags();
    test_process(m, EV);

    assert (!flag_finalD &&
            !flag_gA &&
            !flag_g2 &&
            !flag_eD &&
            !flag_eC &&
            flag_finalC &&
            !flag_eB2 &&
            flag_xB2 &&
            !flag_eA2 &&
            flag_xA2 &&
            !flag_eAB &&
            flag_xAB);

    assert (ab_exit_cnt == 1);

    ufsm_reset_machine(m);
    ufsm_init_machine(m);
    reset_flags(),

    g2_val = false;
    gA_val = true;
    test_process(m, EV);


    assert (!flag_finalD &&
            flag_gA &&
            !flag_g2 &&
            !flag_eD &&
            !flag_eC &&
            !flag_finalC &&
            !flag_eB2 &&
            !flag_xB2 &&
            !flag_eA2 &&
            !flag_xA2 &&
            flag_eAB &&
            !flag_xAB);



    return 0;
}
