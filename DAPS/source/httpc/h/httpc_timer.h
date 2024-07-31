#ifndef _HTTPC_TIMER_H_
#define _HTTPC_TIMER_H_

#include <stdint.h>

typedef struct {
    uint32_t time;
} httpc_time_t;

void httpc_time_init(httpc_time_t *timer);
void httpc_time_countdown_ms(httpc_time_t *timer, uint32_t millisecond);
void httpc_time_start(httpc_time_t *timer);
uint32_t httpc_time_spend(httpc_time_t *start);
uint32_t httpc_time_left(httpc_time_t *end);
uint32_t httpc_time_is_expired(httpc_time_t *timer);
uint32_t httpc_time_get_ms(void);

#endif /* _HTTPC_TIMER_ */
