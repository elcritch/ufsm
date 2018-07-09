#include <assert.h>
#include <stdio.h>
#include <test_conref_input.h>
#include <ufsm.h>
#include "common.h"

static uint32_t eAc = 0;
static uint32_t eCc = 0;
static uint32_t t0c = 0;
static uint32_t t4c = 0;
static uint32_t t3c = 0;
static uint32_t t1c = 0;
static uint32_t xSAc = 0;
static uint32_t eSAc = 0;
static uint32_t xBc = 0;
static uint32_t eBc = 0;

void eA(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { eAc++; }

void eC(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { eCc++; }

void t0(ufsm_sm_t *sm, ufsm_action_t *a) { t0c++; }

void t4(ufsm_sm_t *sm, ufsm_action_t *a) { t4c++; }

void t3(ufsm_sm_t *sm, ufsm_action_t *a) { t3c++; }

void t1(ufsm_sm_t *sm, ufsm_action_t *a) { t1c++; }

void xSA(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { xSAc++; }

void eSA(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { eSAc++; }

void xB(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { xBc++; }

void eB(ufsm_sm_t *sm, ufsm_entry_exit_t *a) { eBc++; }

int main(void)
{
    struct ufsm_machine* m = get_StateMachine1();

    test_init(m);
    ufsm_init_machine(m);

    assert(eAc == 1 && eCc == 0 && t0c == 0 && t1c == 0 && t3c == 0 &&
           t4c == 0 && xSAc == 0 && eSAc == 0 && xBc == 0 && eBc == 0);

    test_process(m, EV1);

    assert(eAc == 1 && eCc == 0 && t0c == 1 && t1c == 0 && t3c == 1 &&
           t4c == 0 && xSAc == 0 && eSAc == 1 && xBc == 0 && eBc == 1);

    test_process(m, EV1);

    assert(eAc == 1 && eCc == 1 && t0c == 1 && t1c == 1 && t3c == 1 &&
           t4c == 1 && xSAc == 1 && eSAc == 1 && xBc == 1 && eBc == 1);

    return 0;
}
