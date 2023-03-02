/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Maxwell Jung, Baron Young
 * Section: Wed 7pm
 * Lab: 6B
 */

#include "stm32l476xx.h"

// configure PA0
void SERVO_Pin_Init(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIOA clock

	GPIOA->MODER &= ~GPIO_MODER_MODE0; // reset moder to 00
	GPIOA->MODER |= GPIO_MODER_MODE0_1; // set moder to AF (10)

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL0; // 3.
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1; // AF2 (TIM5_CH1)

	// GPIO Push-Pull: No pull-up, pull-down (00),
	// Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0; // 4. Set PA0 to no pull-up, no pull-down.
	
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
	GPIOA->OTYPER &=~GPIO_OTYPER_OT0;
}

void TIM5_CH1_Init(){
	/*______________ALL STEP 5.________________*/
	// Set TIM5->CR1 for the counting direction to be up.
	// up (0), down (1)
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	TIM5->CR1 &= ~TIM_CR1_DIR; // 5. set counter direction up (0), 

	// 	Set the prescaler TIM5->PSC to count at 100kHz.
	// 	f_CK_CNT = f_CK_PSC / (PSC[15:0] + 1)
	/*
		f_CK_PSC = 4MHz
		we want  f_CK_CNT = 100kHz 

		100kHz = (4*10^6)/(39+1)
		thus 
		PSC = 39 = 0x27
	*/
	TIM5->PSC &= ~TIM_PSC_PSC;
	TIM5->PSC = 39;

	/*
		Set the TIM5->ARR register
		f_CK_CNT = 100kHz
		Counting Period = (1+ARR)/(f_CK_CNT) 
				  0.02s = (1 + 1999)/100KHz
					ARR = 1999
	*/
	TIM5->ARR &= ~TIM_ARR_ARR;
	TIM5->ARR = 1999;

	/*
		Enable the Output 1 preload enable in TIM5->CCMR1
		0110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
		else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
		TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
	*/
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM5->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
	
	//	Enable the Output 1 preload enable in TIM5->CCMR1:
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;

	// Select the output polarity by clearing the CC1NP in TIM5->CCER :
	//TIM5->CCER |= TIM_CCER_CC1NP;

	// Enable complementary output of Channel 1 (CH1N) by setting the CC1NE bit
	TIM5->CCER |= TIM_CCER_CC1E;

	// Set the main output enable (MOE)
	//TIM5->BDTR |= TIM_BDTR_MOE;
	
	//	Set the output compare register to have a duty cycle of 50%:
	/*
		duty cycle  = CCR/(1+ARR)
		for 50% Duty 
		ARR = 1999
		CCR = 1000 = 0x3E8
	*/
	TIM5->CCR1 &= ~TIM_CCR1_CCR1;
	TIM5->CCR1 = 1000;
		
	TIM5->CR1 |= TIM_CR1_CEN; //Enable timer

}

// function to move the servo to +90 degrees
void Servo_Move_Left_90_Degree(){
	/*
		pulse width = 1ms 
		Duty cycle = CRR/(ARR + 1)
		with a period = 0.02s
		duty cycle = (1ms)/(.02) = 0.05 = 5%
		thus CCR = .05 * 2000 = 100 = 0x64
	*/
	TIM5->CCR1 = 50;
}

// function to move the servo to 0 degrees
void Servo_Move_Center(){
	/*
		pulse width = 1.5ms 
		Duty cycle = CRR/(ARR + 1)
		with a period = 0.02s
		duty cycle = (1.5ms)/(.02) = 0.075 = 7.5%
		thus CCR = .075 * 2000 = 150 = 0x96
	*/
	TIM5->CCR1 = 150;
}

// function to move the servo to -90 degrees
void Servo_Move_Right_90_Degree(){
	/*
		pulse width = 2ms 
		Duty cycle = CRR/(ARR + 1)
		with a period = 0.02s
		duty cycle = (2ms)/(.02) = 0.1 = 10%
		thus CCR = 0.1 * 2000 = 200 = 0xC8
	*/
	TIM5->CCR1 = 250;
}

int main() {
	int i;

	SERVO_Pin_Init();
	TIM5_CH1_Init();
		
    while(1) {
			// Move servo to +90 degrees, 0 degrees, and -90 degrees
			Servo_Move_Right_90_Degree();
			for(i=0;i<200000;i++); 
			Servo_Move_Center();
			for(i=0;i<200000;i++); 
			Servo_Move_Left_90_Degree();
			for(i=0;i<200000;i++);  		// delay
    }
}
