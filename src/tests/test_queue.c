/**
 * uFSM
 *
 * Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <ufsm.h>
#include <assert.h>
#include "common.h"

#include "gen/test_terminate_input.h"

static bool flag_on_data = false;
static bool flag_q_lock = false;
static bool flag_q_unlock = false;

void xA(ufsm_machine_t *m, ufsm_state_t *t) {}
void eA(ufsm_machine_t *m, ufsm_state_t *t) {}

void on_data()
{
    flag_on_data = true;
}

void q_lock()
{
    flag_q_lock = true;
}

void q_unlock()
{
    flag_q_unlock = true;
}

int main(int argc, char **argv)
{
    uint32_t err = UFSM_OK;
    uint32_t data[4];
    uint32_t i = 0;
    struct ufsm_machine *m = get_StateMachine1();

    struct ufsm_queue *q = ufsm_get_queue(m);
    assert (ufsm_queue_init(q, 4, data) == UFSM_OK);


    q->on_data = on_data;
    q->lock = q_lock;
    q->unlock = q_unlock;


    err = ufsm_queue_put(q, 1);
    assert (err == UFSM_OK);
    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_OK && i == 1);

    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_ERROR_QUEUE_EMPTY);

    err = ufsm_queue_put(q, 1);
    err = ufsm_queue_put(q, 2);
    err = ufsm_queue_put(q, 3);
    err = ufsm_queue_put(q, 4);
    
    assert (err == UFSM_OK);
    err = ufsm_queue_put(q, 5);
    assert (err == UFSM_ERROR_QUEUE_FULL);

    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_OK && i == 1);
    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_OK && i == 2);
    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_OK && i == 3);
    err = ufsm_queue_get(q, &i);
    assert ( err == UFSM_OK && i == 4);

    assert (flag_on_data && flag_q_unlock && flag_q_lock);
    return 0;
}
