#define SYSCTL_RCGC2_R 					(*((volatile unsigned long *) 0x400FE108))
#define GPIO_PORTA_DATA_R				(*((volatile unsigned long *) 0x400043FC))
#define GPIO_PORTA_DIR_R				(*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_AFSEL_R			(*((volatile unsigned long *) 0x40004420))
#define GPIO_PORTA_PUR_R 				(*((volatile unsigned long *) 0x40004510))
#define GPIO_PORTA_DEN_R				(*((volatile unsigned long *) 0x4000451c))
#define GPIO_PORTA_CR_R					(*((volatile unsigned long *) 0x40004524))
#define GPIO_PORTA_AMSEL_R			(*((volatile unsigned long *) 0x40004528))
#define GPIO_PORTA_PCTL_R				(*((volatile unsigned long *) 0x4000452c))
#define GPIO_PORTA_IS_R         (*((volatile unsigned long *)0x40004404))
#define GPIO_PORTA_IBE_R        (*((volatile unsigned long *)0x40004408))
#define GPIO_PORTA_IEV_R        (*((volatile unsigned long *)0x4000440C))
#define GPIO_PORTA_IM_R         (*((volatile unsigned long *)0x40004410))
#define GPIO_PORTA_RIS_R        (*((volatile unsigned long *)0x40004414))
#define GPIO_PORTA_ICR_R        (*((volatile unsigned long *)0x4000441C))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
	
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))
#define NVIC_PRI7_R             (*((volatile unsigned long *)0xE000E41C))
	
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void PortA_init(void)
{
	SYSCTL_RCGC2_R = 0x3f;			//Enable clock for portA
	GPIO_PORTA_DIR_R = 0x04;		//PA2 output and PA3 input
	GPIO_PORTF_DIR_R = 0x0E;		//
	GPIO_PORTA_DEN_R = 0x0c;
	GPIO_PORTF_DEN_R = 0x0E;
	GPIO_PORTA_PUR_R = 0x08;		// Pull up
}
void Edge_init(void)
{
	GPIO_PORTA_IS_R = 0x00;		//Select level or edge (1--level) (0--edge)  pg. no.: 667
	GPIO_PORTA_IBE_R = 0x00;	//select falling or rising both (1--both)
	GPIO_PORTA_IEV_R = 0x00;	//select either falling or rising (1--rising) (0--falling)
	GPIO_PORTA_IM_R = 0x08;		//interrupt enable PA3
	GPIO_PORTA_ICR_R = 0x08;	//Enable ackonwledge flag;
	NVIC_EN0_R = 0x00000001;	//Select interrupt for PortA (PortA interrupt number is 0,portB 1,portc 2,portD 3,portE 4,portf 30) pg no.:120
	EnableInterrupts();
}
void GPIOPortA_Handler(void)
{
	GPIO_PORTA_ICR_R = 0x08;
	GPIO_PORTA_DATA_R ^=0x04;
}
int main()
{
	PortA_init();
	Edge_init();
	
	while(1)
	{
		WaitForInterrupt();
	}
	return 0;
}
