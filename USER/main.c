#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "adc.h"
#include "rtc.h"
#include "m26.h"
#include "status_manager.h"
#include "chipid_chipurl.h"
#include "data_parser.h"
#include "pwm.h"
#include "relay.h"
#include "aes.h"

extern uint8_t get_Air_adc_val_f;
extern u32 ChipUniqueID[3];

extern char expressText[200];  //��Ŵ����ܵ��������ݣ����建���С�����û����������ݳ����Լ������޸�
extern char cipherText[400];//����Ѽ��ܵ��������ݣ����建���С�����û����ܺ�����ݳ����Լ������޸�
u8 datatemp[] = {"AT+QIOPEN=\"TCP\",\"000.000.000.000\",\"12345\"\r\n"};

int main(void)
{	 
//	int i;
	char deviceid[12];
	uint8_t  breath_LED_f = 0;
	uint16_t Air_adc_val;
	float Air_vol;
	int run_status;
	uint16_t val = 0;
	uint8_t dir = 1;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	TIM2_Int_Init(5000-1,7200-1);  //watchdog timer
	TIM5_Int_Init(1000-1,7200-1);  //heartbeat timer
	Adc_Init();
	usart3_init(115200);
	m26_Powerkey_Init();
	KEY_Init();
	EXTIX_Init();
	Relay_Init();
	Relay_Status_Check_Init();
	Relay_Led_Init();
	
	Init_PWM();
	Get_ChipID();

	sprintf(deviceid,"%x%x%x",ChipUniqueID[2],ChipUniqueID[1],ChipUniqueID[0]);
	printf("device id is: %24s\n",deviceid);
	memset(expressText ,0 ,200);
	memset(cipherText , 0 ,200);
	strcpy(expressText , deviceid);
	printf("device id is: %s\n",expressText);
//	memset(expressText ,0 ,200);
//	memset(cipherText , 0 ,400);
//	strcpy(expressText , (char *)ChipUniqueID);
//	AES_Encrypt(expressText  , strlen(expressText) , cipherText);	//use aesKey encrypt
//	
//	 printf("AES_Encrypt data is:   %s\r\n",cipherText);
//	 
//	 
//	memset(expressText ,0 ,200);
//	AES_Decrypt(cipherText , (sizeof(cipherText)) , expressText);//use aesKey decrypt
//	printf("AES_Decrypt data is:   %x\r\n",(unsigned int)expressText);	
	RTC_Init();
//	RTC_Alarm_Set(42);	
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,sizeof(datatemp));
	printf("read flash data is: %s\n",datatemp);
	set_status_flag(POWERON);

	while(1) 
	{	
//		KEY_Scan(0);
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
		
		Data_Parser();

		
//		if (get_Air_adc_val_f)
//		{
			Air_adc_val = Get_Adc_Average(2,3);
			Air_vol = Air_adc_val*3.45/4096;
			if (Air_vol > 2.2)
				breath_LED_f = 1;
//		}
		//breath led control
		if(breath_LED_f)
		{
			if(dir)
				val++;
			else
				val--;
			if(val > 100) dir=0;
			if(val == 0 ) dir=1;
			TIM3->CCR3 = val;
		}
		else
		{
			TIM3->CCR3 = 150;
		}
		breath_LED_f = 0;
		
		//relay led control
		if(0 == RELAYCHKSTUS)
		{
			RELAYLED = 1;
		}
		else
		{
			RELAYLED = 0;
		}
	}
}
