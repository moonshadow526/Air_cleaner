#include "data_parser.h"

extern uint8_t timer2off_cleaner;

uint8_t Data_Parser(void)
{
	static int timeval;
	u8 tmp1=0,tmp2=0;
	int i,tempCountValue,time=0;
	USART3_RX_STA = 0;
	if(Usart_recive_cmpl == 1)
	{	
		printf("recive data is: %s \n",USART3_RX_BUF);
		if('{' == USART3_RX_BUF[0]&&'}'== USART3_RX_BUF[strlen(USART3_RX_BUF)-1])
		{
			if(strstr(USART3_RX_BUF,"operation\":\"open"))
			{
				printf("timer data get\n");
				for(i = 0; i<(strlen(USART3_RX_BUF)); i++)
				{
					printf("usart rxbuf %d %c \n",i,USART3_RX_BUF[i]);
					if('\"'==USART3_RX_BUF[i])
					{
						tmp1++;
						printf("tmp1 %d\n",tmp1);
						if(7 == tmp1)
						{
							tmp2=i+1;
							printf("tmp2 %d\n",tmp2);
						}
						if(8 == tmp1)
						{
							tmp1 = i;
							while((tmp1-tmp2)>0)
							{
								time=time*10+(int)USART3_RX_BUF[tmp2++]-'0';
								printf("time is %d\n",time);
							}
						}
					}
				}
				tempCountValue = RTC_GetCounter();
//				printf("alarmstatus %d \n",time);
				if(0==RELAYOUT&&time!=0)
				{
					printf("NOW: %d\n",tempCountValue);
					RELAYOUT = 1;
					RTC_Alarm_Set(tempCountValue+time*60);
					printf("ALARM: %d\n",tempCountValue+time*60);
					timeval = tempCountValue+time*60;
					time = 0;
				}
				else if(1==RELAYOUT&&time!=0)
				{
					timeval+=time*60;
					RTC_Alarm_Set(timeval);
					printf("ALARM: %d\n",timeval);
				}
			}
		}
			for(i = 0; i<(sizeof(USART3_RX_BUF)/sizeof(u8)); i++)
			{
				USART3_RX_BUF[i] = 0;
			}
			Usart_recive_cmpl = 0;
		}
	
	if(timer2off_cleaner)
	{
		RELAYOUT = 0;
		timer2off_cleaner = 0;
		timeval = 0;
	}
	return 0;
}
