#ifndef _STATUS_MANAGER_H_
#define _STATUS_MANAGER_H_

enum Status{
	POWERON=1, TCP_CONNECT, SEND_MSG, HEART_BIT
};


void set_status_flag(enum Status status);
int get_status_flag(void);

#endif
