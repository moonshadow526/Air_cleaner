#ifndef _DATA_PARSER_H_
#define _DATA_PARSER_H_

#define HEARTBIT 2
#define OPENPOWER 1
#define CLOSEPOWER 0

#include "sys.h"
#include "usart3.h"
#include "usart.h"
#include "relay.h"
#include "rtc.h"

extern uint8_t Usart_recive_cmpl;

uint8_t Data_Parser(void);

#endif
