#include "timer_cap.h"
#include "stm32f10x_tim.h"
//#include "delay.h"

u8 TIM3CH1_CAPTURE_STA = 0;	//通道1输入捕获标志，高两位做捕获标志，低6位做溢出标志		
u16 TIM3CH1_CAPTURE_UPVAL;
u16 TIM3CH1_CAPTURE_DOWNVAL;

u8 TIM3CH2_CAPTURE_STA = 0;	//通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志		
u16 TIM3CH2_CAPTURE_UPVAL;
u16 TIM3CH2_CAPTURE_DOWNVAL;

u8 TIM3CH3_CAPTURE_STA = 0;	//通道3输入捕获标志，高两位做捕获标志，低6位做溢出标志		
u16 TIM3CH3_CAPTURE_UPVAL;
u16 TIM3CH3_CAPTURE_DOWNVAL;

u8 TIM3CH4_CAPTURE_STA = 0;	//通道1输入捕获标志，高两位做捕获标志，低6位做溢出标志		
u16 TIM3CH4_CAPTURE_UPVAL;
u16 TIM3CH4_CAPTURE_DOWNVAL;

u32 tempup1 = 0;	//捕获总高电平的时间
u32 tempup2 = 0;	//捕获总高电平的时间
u32 tempup3 = 0;	//捕获总高电平的时间
u32 tempup4 = 0;	//捕获总高电平的时间
u32 tim3_T1;
u32 tim3_T2;
u32 tim3_T3;
u32 tim3_T4;

int pwmout1, pwmout2, pwmout3, pwmout4; 				//输出占空比

//定时器4通道1输入捕获配置

TIM_ICInitTypeDef TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟

	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PD12 下拉输入 
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_0
			| GPIO_Pin_1;  //PB6,8,9 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6,7,8,9 输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);//PB6,8,9  下拉
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);//PB6,8,9  下拉

	//初始化定时器4 TIM3，该部分配置主要是为PWM输出服务 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM3输入捕获参数 通道1
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//初始化TIM3输入捕获参数 通道2
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//初始化TIM3输入捕获参数 通道3
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//初始化TIM3输入捕获参数 通道4
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//中断分组初始化
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,
			ENABLE);   //不允许更新中断，允许CC1IE,CC2IE,CC3IE,CC4IE捕获中断	

	TIM_Cmd(TIM3, ENABLE); 		//使能定时器4

}

//定时器4中断服务程序,用CH1作为超声波转换的通道 即PB6端口
void TIM3_IRQHandler(void)
{
	if ((TIM3CH1_CAPTURE_STA & 0X80) == 0) 		//还未成功捕获	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) 		//捕获1发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//清除中断标志位
			if (TIM3CH1_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM3);//记录下此时的定时器计数值
				if (TIM3CH1_CAPTURE_DOWNVAL < TIM3CH1_CAPTURE_UPVAL)
				{/* 如果计数器初始值大于末尾值，说明计数器有溢出 */
					tim3_T1 = 65535;
				}
				else
				tim3_T1 = 0;  
				tempup1 = TIM3CH1_CAPTURE_DOWNVAL - TIM3CH1_CAPTURE_UPVAL
						+ tim3_T1;		//得到总的高电平的时间
				//pwmout1 = tempup1;		//总的高电平的时间
				tempup1 =tempup1 *172/10000;//计算距离&&UltrasonicWave_Distance<85
			
				
				
				TIM3CH1_CAPTURE_STA = 0;		//捕获标志位清零，这一步很重要！
				TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获		  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM3);		//获取上升沿数据
				TIM3CH1_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
	//delay_ms(50);
	if ((TIM3CH2_CAPTURE_STA & 0X80) == 0)		//还未成功捕获	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)		//捕获2发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);		//清除中断标志位
			if (TIM3CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM3);//记录下此时的定时器计数值
				if (TIM3CH2_CAPTURE_DOWNVAL < TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T2 = 65535;
				}
				else
					tim3_T2 = 0;
				tempup2 = TIM3CH2_CAPTURE_DOWNVAL - TIM3CH2_CAPTURE_UPVAL
						+ tim3_T2;		//得到总的高电平的时间
				//pwmout2 = tempup2;		//总的高电平的时间
				tempup2 =tempup2 *172/10000;//计算距离&&UltrasonicWave_Distance<85
				TIM3CH2_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获		  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM3);		//获取上升沿数据
				TIM3CH2_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}

	if ((TIM3CH3_CAPTURE_STA & 0X80) == 0)		//还未成功捕获	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)		//捕获3发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);		//清除中断标志位
			if (TIM3CH3_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM3);//记录下此时的定时器计数值
				if (TIM3CH3_CAPTURE_DOWNVAL < TIM3CH3_CAPTURE_UPVAL)
				{
					tim3_T3 = 65535;
				}
				else
					tim3_T3 = 0;
				tempup3 = TIM3CH3_CAPTURE_DOWNVAL - TIM3CH3_CAPTURE_UPVAL
						+ tim3_T3;		//得到总的高电平的时间
				pwmout3 = tempup3;		//总的高电平的时间
				TIM3CH3_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获		  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM3);		//获取上升沿数据
				TIM3CH3_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}

	if ((TIM3CH4_CAPTURE_STA & 0X80) == 0)		//还未成功捕获	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)		//捕获4发生捕获事件
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);		//清除中断标志位
			if (TIM3CH4_CAPTURE_STA & 0X40)		//捕获到一个下降沿
			{
				TIM3CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM3);//记录下此时的定时器计数值
				if (TIM3CH4_CAPTURE_DOWNVAL < TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T4 = 65535;
				}
				else
					tim3_T4 = 0;
				tempup4 = TIM3CH4_CAPTURE_DOWNVAL - TIM3CH4_CAPTURE_UPVAL
						+ tim3_T4;		//得到总的高电平的时间
				pwmout4 = tempup4;		//总的高电平的时间
				TIM3CH4_CAPTURE_STA = 0;		//捕获标志位清零
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置为上升沿捕获		  
			}
			else //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				TIM3CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM3);		//获取上升沿数据
				TIM3CH4_CAPTURE_STA |= 0X40;		//标记已捕获到上升沿
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);//设置为下降沿捕获
			}
		}
	}
}
