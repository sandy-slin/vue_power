#include "userapp.h"

/* 记录当前电源开关的状态 */
static POWER_State GPIO_PowerPinStatus = POWER_DISABLE;


/*********
1.CM3模块电源通断控制函数
2.注意：尽量只用此函数来控制CM3的电源通断
*********/
void PowerControl(POWER_State status)
{
	if (status != POWER_DISABLE)
  {
    HAL_GPIO_WritePin(GPIO_POWER_GPIO_Port,GPIO_POWER_Pin,GPIO_PIN_SET);
		GPIO_PowerPinStatus = POWER_ENABLE;
  }
  else
  {
    HAL_GPIO_WritePin(GPIO_POWER_GPIO_Port,GPIO_POWER_Pin,GPIO_PIN_RESET);
		GPIO_PowerPinStatus = POWER_DISABLE;
  }
}




/*********
1.open power-led
2.switch on boot usb
3.open power
*********/
void UsbBoot(void)
{
	HAL_GPIO_WritePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIO_USB_BOOT_GPIO_Port,GPIO_USB_BOOT_Pin,GPIO_PIN_SET);
	HAL_Delay(10);
	PowerControl(POWER_ENABLE);
	HAL_Delay(10);
	while(1)
	{
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin);
	}
}



void PowerOn_normal(void)
{
	GPIO_PinState power_key;
  GPIO_PinState softpower_key;
	
	uint32_t PowerKeyPressTime = 0;
	uint32_t SoftKeyPressTime = 0;
	uint32_t PowerKeyReleaseTime = 0;
	uint32_t SoftKeyReleaseTime = 0;
  
  while (1) 
  {
    power_key = HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port,GPIO_KEY_Pin);
    softpower_key = HAL_GPIO_ReadPin(GPIO_SOFTKEY_GPIO_Port,GPIO_SOFTKEY_Pin);
    
		/* 获取softpower_key和power_key按键按下的时常 */
    if (softpower_key == GPIO_PIN_SET)
		{
			SoftKeyPressTime = HAL_GetTick() - SoftKeyReleaseTime;
		}
    else
    {
			SoftKeyReleaseTime = HAL_GetTick();
			SoftKeyPressTime = 0;
    }
		
    /* logic for power key */
    if (power_key == GPIO_PIN_RESET)
		{
			PowerKeyPressTime = HAL_GetTick() - PowerKeyReleaseTime;
		}
    else
    {
			PowerKeyReleaseTime = HAL_GetTick();
			PowerKeyPressTime = 0;
    }
    
		/*
		**1.开着机并按下按键超1s--->拉高电平通知CM3准备4s后强制关机**
		**2.开着机并按下按键超5s--->强制关机--->等待释放按键
		**3.开着机并接收到来自CM3的关机命令--->3s后强制关机
		*/
		if(GPIO_PowerPinStatus == POWER_ENABLE)
		{
			if((PowerKeyPressTime > TIMER_HW_PD_FORCE_TRIGGER)&&(PowerKeyPressTime <= TIMER_HW_PD_FORCE))
			{
				//falling edge to trigger CM3
				HAL_GPIO_WritePin(GPIO_POWER_TRIGGER_GPIO_Port,GPIO_POWER_TRIGGER_Pin,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIO_POWER_TRIGGER_GPIO_Port,GPIO_POWER_TRIGGER_Pin,GPIO_PIN_RESET);
			}
			else if(PowerKeyPressTime > TIMER_HW_PD_FORCE)
			{
				PowerControl(POWER_DISABLE);
				HAL_GPIO_WritePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin,GPIO_PIN_SET);
				//HAL_GPIO_WritePin(GPIO_POWER_TRIGGER_GPIO_Port,GPIO_POWER_TRIGGER_Pin,GPIO_PIN_RESET);
				while(!HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port,GPIO_KEY_Pin));
			}
			
			if( SoftKeyPressTime > TIME_SW_PD )
			{
				PowerControl(POWER_DISABLE);
				HAL_GPIO_WritePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin,GPIO_PIN_SET);
				//HAL_GPIO_WritePin(GPIO_POWER_TRIGGER_GPIO_Port,GPIO_POWER_TRIGGER_Pin,GPIO_PIN_RESET);
			}
		}
		/*
		**1.关着机并按下按键超1.2s--->开机**
		**2.关着机并按下按键超5s--->
		**3.等待释放按键
		*/
		else if(GPIO_PowerPinStatus == POWER_DISABLE)
		{
			if(PowerKeyPressTime > TIME_HW_PD)
			{
				PowerControl(POWER_ENABLE);
				HAL_GPIO_WritePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin,GPIO_PIN_RESET);
				while(!HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port,GPIO_KEY_Pin));
			}
		}
	}
}



void PowerManager(void)
{
	/** 如果按住电源键后接通电源，则进入USB BOOT启动模式，否则进入正常开机启动流程 **/
  if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port,GPIO_KEY_Pin))
  {
		UsbBoot();
  }
	else
	{
		PowerOn_normal();
	}
}



/*
**1.关闭电源
**2.设置CM3预关机通知信号为低,关闭USB BOOT模式
**3.闪烁一下LED灯表示已经插入电源线，之后熄灭LED灯
*/
void PowerManagerInit(void)
{
	PowerControl(POWER_DISABLE);
	HAL_GPIO_WritePin(GPIO_POWER_TRIGGER_GPIO_Port,GPIO_POWER_TRIGGER_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO_USB_BOOT_GPIO_Port,GPIO_USB_BOOT_Pin,GPIO_PIN_RESET);
	
	for(uint8_t i=0;i<3;i++){
		HAL_GPIO_TogglePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin);
		HAL_Delay(300);
	}
	
	HAL_GPIO_WritePin(GPIO_LED_GPIO_Port,GPIO_LED_Pin,GPIO_PIN_SET);
}










