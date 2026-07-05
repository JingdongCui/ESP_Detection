#pragma once

#include "sorter_core/sorter_scheduler.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int sorter_protocol_format_event(const sorter_event_t *event, char *buf, size_t len);

#ifdef __cplusplus
}
#endif
