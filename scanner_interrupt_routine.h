/**
	*@author Md Sabbir Bin Zaman
	*@property of Embedded System Research & Advanced Protoryping Lab(ESARP)
*/


//ADC_HandleTypeDef hadc1;
//uint16_t totalSample=0,adcConversion=0,length;

//uint8_t timeKeeper=0;//,buffer[256]

/**
	interrupt routine for Bluetooth reception using USART2.  
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//uint8_t i;
		if(huart->Instance==USART2){
		if(rx_index<2){
			if((Rx_data[0]!=10)&&(Rx_data[0]!=13))
			Rx_Buffer[rx_index++]=Rx_data[0];
		} 
		if(rx_index==2){
			rx_index=0;
		//	printf(Rx_Buffer);
			initializeScanner();
		}
		
		}
			
	
		HAL_UART_Receive_IT(&huart2,&Rx_data[0],1);
}
/**
	Timer Interrupt routine to take care of LED blinking rate and (phone-scanner)initialization timeout. 
	TIM2: for adc sampling rate
	TIM4 for timeWindow for total adc conversion time. at interrupt it will reset TIM2 and ADC.
	TIM3 for LED blinking rate controlling
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	
	if(htim->Instance==TIM4){//timeWindow timer interrupt
	/**	
		{
		//HAL_TIM_Base_Stop_IT(&htim2);
		if(timeKeeper>=timeWindow){
		timeKeeper=0;

		//HAL_TIM_Base_Stop(&htim2);
			
			//HAL_Delay(10);
		resetTimeWindow();
		///printf("time window elapsed");
		}
		else{
			++timeKeeper;
		}
	}*/
		resetTimeWindow();
		
	}else if(htim->Instance==TIM3){// 5Hz LED blinker timer interrput
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
	}else if(htim->Instance==TIM5){// 1Hz LED blinker timer interrput 
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
		//printf("blink");
		//transmitBluetooth()
	}else if(htim->Instance==TIM2){//sampling rate timer interrupt
		//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
		totalSample++;
		HAL_ADC_Start_IT(&hadc1);
		//HAL_ADC_Start_DMA(&hadc1, (uint32_t*) buffer, 256);
		
		
	}
	
}

/**
	ADC conversion interrput routine: triggered by TIM2, transfer via transmit2Bluetooth(uint16_t)
	adcValue declared in scanner.h
	transmit2Bluetooth defined in scanner.h
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance==ADC1){
			
		++adcConversion;
		adcValue=HAL_ADC_GetValue(hadc);
		//transmit2Bluetooth(adcValue);
		//if(dataCheckDone==1)
		storeToBuffer(adcValue);
		//storeToBuffer(adcConversion);
//	else
	//	storeDataForDataCheck(adcValue);
		
		
	}
}