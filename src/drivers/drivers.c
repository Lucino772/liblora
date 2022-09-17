/*
 * Created on Tue Sep 17 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#include "_drivers.h"

#include <stdlib.h>

void liblora_driver_delete(liblora_driver_t *driver)
{
    if (driver == NULL)
        return;

    if (driver->config != NULL)
        free(driver->config);

    if (driver != NULL)
        free(driver);
}