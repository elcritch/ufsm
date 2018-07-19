/**
 * uFSM
 *
 * Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause

 *
 */

#include <ufsm.h>

static ufsm_status_t _ufsm_queue_put(struct ufsm_queue *q, struct ufsm_event item)
{
    ufsm_status_t err = UFSM_OK;

    if (q->lock)
        q->lock();

    if (q->size < q->no_of_elements) {
        q->data[q->head] = item;
        q->size++;
        q->head++;

        if (q->on_data)
            q->on_data();

        if (q->head >= q->no_of_elements)
            q->head = 0;
    }
    else
    {
        err = UFSM_ERROR_QUEUE_FULL;
    }

    if (q->unlock)
        q->unlock();

    return err;
}

static ufsm_status_t _ufsm_queue_get(struct ufsm_queue *q, struct ufsm_event *item)
{
    ufsm_status_t err = UFSM_OK;

    if (q->lock)
        q->lock();

    if (q->size) {
        *item = q->data[q->tail];
        q->size--;
        q->tail++;

        if (q->tail >= q->no_of_elements)
            q->tail = 0;
    }
    else
    {
        err = UFSM_ERROR_QUEUE_EMPTY;
    }

    if (q->unlock)
        q->unlock();

    return err;
}

ufsm_status_t ufsm_queue_put(struct ufsm_queue *q, event_t ev)
{
    struct ufsm_event item = {.ev = ev, .data = NULL};
    return _ufsm_queue_put(q, item);
}

ufsm_status_t ufsm_queue_get(struct ufsm_queue *q, event_t *ev)
{
    struct ufsm_event item;
    ufsm_status_t err = _ufsm_queue_get(q, &item);
    *ev = item.ev;
    return err;
}

ufsm_status_t ufsm_queue_put_item(struct ufsm_queue *q, struct ufsm_event item)
{
    return _ufsm_queue_put(q, item);
}

ufsm_status_t ufsm_queue_get_item(struct ufsm_queue *q, struct ufsm_event *item)
{
    return _ufsm_queue_get(q, item);
}

ufsm_status_t ufsm_queue_init(struct ufsm_queue *q, uint32_t no_of_elements,
                                            struct ufsm_event *data)
{
    q->head = 0;
    q->tail = 0;
    q->data = data;
    q->size = 0;
    // q->on_data = NULL;
    // q->lock = NULL;
    // q->unlock = NULL;
    q->no_of_elements = no_of_elements;

    return UFSM_OK;
}
