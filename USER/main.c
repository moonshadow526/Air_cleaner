#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "adc.h"
#include "m26.h"
#include "status_manager.h"
#include "chipid_chipurl.h"
#include "pwm.h"

extern uint8_t get_Air_adc_val_f;

int main(void)
{	 
	uint16_t Air_adc_val;
	float Air_vol;
	int run_status;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为9600
	TIM2_Int_Init(50000-1,7200-1);  //watchdog timer
	TIM5_Int_Init(10000-1,7200-1);  //heartbeat timer
	Adc_Init();
	usart3_init(115200);
	m26_Powerkey_Init();
	
	LED_Init();			     //LED端口初始化
	Init_PWM();
	Get_ChipID();		 	
	set_status_flag(POWERON);
	while(1) 
	{	
		run_status = get_status_flag();
		switch(run_status)
		{
			case POWERON:
			{
				m26_resest();
				set_status_flag(TCP_CONNECT);
				break;
			}
			case TCP_CONNECT:
			{
				set_status_flag(SEND_MSG);
				m26_build_tcp_connect();	
				break;
			}
			case SEND_MSG:
			{
				m26_send_msg();
				set_status_flag(HEART_BIT);
				break;
			}
			case HEART_BIT:
			{
	//			m26_build_tcp_connect();
				break;
			}
			default:
			{
				break;
			}
		}
		if(USART3_RX_STA == 1)
		{
			printf("recive data is: %s \n",USART3_RX_BUF);
			USART3_RX_STA = 0;
		}
		
		if (get_Air_adc_val_f)
		{
			Air_adc_val = Get_Adc_Average(2,3);
			Air_vol = Air_adc_val*3.45/4096;
			if (Air_vol > 2.2)
			breath_LED();
			TIM3->CCR3 = 0;
		}
		
		delay_ms(400);
	}
}
