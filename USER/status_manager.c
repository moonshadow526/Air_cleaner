#include "status_manager.h"
static int status_flag = 0;

void set_status_flag(enum Status status)
{
	status_flag = status;
}

int get_status_flag(void)
{
	return status_flag;
}
