/*
 * Copyright (c) 1996-2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include "idigi_api.h"
#include "platform.h"

int app_os_malloc(size_t const size, void ** ptr)
{
    int status=-1;

    *ptr = malloc(size);
    if (*ptr != NULL)
    {
        status = 0;
    }
    else
    {
        APP_DEBUG("app_os_malloc: Failed to malloc\n");
    }

    return status;
}

void app_os_free(void * const ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
    }
    else
    {
        APP_DEBUG("app_os_free: called with NULL\n");
    }

    return;
}

int app_os_get_system_time(unsigned long * const uptime)
{
    time((time_t *)uptime);

    return 0;
}

void app_os_sleep(unsigned int const timeout_in_seconds)
{
    unsigned int const timeout_in_microseconds = timeout_in_seconds * 1000000;

    usleep(timeout_in_microseconds);
    return;
}

idigi_callback_status_t app_os_handler(idigi_os_request_t const request,
                                        void * const request_data, size_t const request_length,
                                        void * response_data, size_t * const response_length)
{
    idigi_callback_status_t status = idigi_callback_continue;
    int ret;

    UNUSED_ARGUMENT(request_length);
    UNUSED_ARGUMENT(response_length);

    switch (request)
    {
    case idigi_os_malloc:
        ret    = app_os_malloc(*((size_t *)request_data), (void **)response_data);
        status = (ret == 0) ? idigi_callback_continue : idigi_callback_busy;
        break;

    case idigi_os_free:
        app_os_free(request_data);
        status = idigi_callback_continue;
        break;

    case idigi_os_system_up_time:
        ret    = app_os_get_system_time((unsigned long *)response_data);
        status = (ret == 0) ? idigi_callback_continue : idigi_callback_abort;
        break;

    case idigi_os_sleep:
        app_os_sleep(*((unsigned int *)request_data));
        status = idigi_callback_continue;
        break;

    default:
        APP_DEBUG("idigi_os_callback: unrecognized request [%d]\n", request);
        break;
    }

    return status;
}


