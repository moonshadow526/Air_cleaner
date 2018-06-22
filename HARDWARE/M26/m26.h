#ifndef _M26_H_
#define _M26_H_

#define M26_POWERKEY PBout(14)

void m26_resest(void);
void m26_call_test(void);
void m26_ring_test(void);
void m26_build_tcp_connect(void);
void m26_send_msg(void);
void m26_Powerkey_Init(void);

#endif


