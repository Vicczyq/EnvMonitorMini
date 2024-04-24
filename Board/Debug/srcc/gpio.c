//===========================================================================
//文件名称：gpio.c
//功能概要：GPIO底层驱动构件源文件
//版权所有：SD-Arm(sumcu.suda.edu.cn)
//版本更新：20181201-20200221
//芯片类型：STM32
//===========================================================================

#include "gpio.h"

//GPIO口基地址放入常数数据组GPIO_ARR[0]~GPIO_ARR[5]中
GPIO_TypeDef * GPIO_ARR[] =
       {(GPIO_TypeDef *)GPIOA_BASE,(GPIO_TypeDef *)GPIOB_BASE,
		(GPIO_TypeDef *)GPIOC_BASE,(GPIO_TypeDef *)GPIOD_BASE,
		(GPIO_TypeDef *)GPIOE_BASE,(GPIO_TypeDef *)GPIOH_BASE};

//====定义扩展中断IRQ号对应表====
IRQn_Type table_irq_exti[7] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn,
		EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn};

//内部函数声明
void gpio_get_port_pin(uint16_t port_pin,uint8_t* port,uint8_t* pin);

//=====================================================================
//函数名称：gpio_init
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//         dir：引脚方向（0=输入，1=输出,可用引脚方向宏定义）
//         state：端口引脚初始状态（0=低电平，1=高电平）
//功能概要：初始化指定端口引脚作为GPIO引脚功能，并定义为输入或输出，若是输出，
//         还指定初始状态是低电平或高电平
//=====================================================================
void gpio_init(uint16_t port_pin, uint8_t dir, uint8_t state)
{
	GPIO_TypeDef *gpio_ptr;    //声明gpio_ptr为GPIO结构体类型指针
	uint8_t port,pin;    //声明端口port、引脚pin变量
	uint32_t temp;       //临时存放寄存器里的值
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

	//使能相应GPIO时钟
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN<<(port * 1u));

	//清GPIO模式寄存器对应引脚位
	temp = gpio_ptr->MODER;
	temp &= ~(GPIO_MODER_MODE0 << (pin * 2u));

	if(dir == 1)  //定义为输出引脚
	{
		temp |= (GPIO_OUTPUT << (pin * 2u));
		gpio_ptr->MODER = temp;
		gpio_set(port_pin,state);    //调用gpio_set函数，设定引脚初始状态
	}
	else  //定义为输入引脚
	{
		temp |= (GPIO_INPUT << (pin * 2u));
		gpio_ptr->MODER = temp;
	}
}

//=====================================================================
//函数名称：gpio_set
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//          state：希望设置的端口引脚状态（0=低电平，1=高电平）
//功能概要：当指定端口引脚被定义为GPIO功能且为输出时，本函数设定引脚状态
//=====================================================================
void gpio_set(uint16_t port_pin, uint8_t state)
{
	//局部变量声明
	GPIO_TypeDef *gpio_ptr;    //声明port_ptr为GPIO结构体类型指针（首地址）
	uint8_t port,pin;        //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

	//根据state，设置对应引脚状态
	if(1 == state)    //高电平(该引脚对应置位寄存器置1)
		gpio_ptr->BSRR = (uint32_t)(1u<<pin);
	else              //低电平(该引脚对应重置寄存器置1)
		gpio_ptr->BRR = (uint32_t)(1u<<pin);
}

//=====================================================================
//函数名称：gpio_get
//函数返回：指定端口引脚的状态（1或0）
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时，本函数获取指定引脚状态
//=====================================================================
uint8_t gpio_get(uint16_t port_pin)
{
	//局部变量声明
	GPIO_TypeDef *gpio_ptr;    //声明port_ptr为GPIO结构体类型指针（首地址）
	uint8_t port,pin;        //声明端口port、引脚pin变量
	uint32_t temp;
	uint8_t value;
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

	//获得状态寄存器的值
	temp = gpio_ptr->MODER;

	if( (temp & (2u<<(pin*2)))>>(pin*2) == 1u )//GPIO输出
	{
		//读取Output data寄存器对应引脚的值
		temp = gpio_ptr->ODR;
		if((temp & (1u<<pin)) != 0x00u)
			value = 1;
		else
			value = 0;
	}
	else                                       //GPIO输入
	{
		//读取Input data寄存器对应引脚的值
		temp = gpio_ptr->IDR;
		if((temp & (1u<<pin)) != 0x00u)
			value = 1;
		else
			value = 0;
	}

	return value;
}

//=====================================================================
//函数名称：gpio_reverse
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//功能概要：当指定端口引脚被定义为GPIO功能且为输出时，本函数反转引脚状态
//=====================================================================
void gpio_reverse(uint16_t port_pin)
{
	//局部变量声明
	GPIO_TypeDef *gpio_ptr;    //声明port_ptr为GPIO结构体类型指针（首地址）
	uint8_t port,pin;        //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

    //判断引脚输出状态
	if ((gpio_ptr->ODR & (1u<<pin)) != 0x00u)
		//高电平，则反转为低电平
		gpio_ptr->BRR = (uint32_t)(1u<<pin);
	else
		//低电平，则反转为高电平
		gpio_ptr->BSRR = (uint32_t)(1u<<pin);
}

//=====================================================================
//函数名称：gpio_pull
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//       pullselect：下拉/上拉（PULL_DOWN=下拉，PULL_UP=上拉）
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时，本函数设置引脚下拉/上拉
//=====================================================================
void gpio_pull(uint16_t port_pin, uint8_t pullselect)
{
	GPIO_TypeDef *gpio_ptr;    //声明gpio_ptr为GPIO结构体类型指针
	uint8_t port,pin;    //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

	//设定为高速输出状态
	gpio_ptr->OSPEEDR |= (uint32_t)(HIGH_SPEED << (pin * 2u));

	if(1 == pullselect)
		//引脚拉高电平
		gpio_ptr->PUPDR |= (uint32_t)(PULL_UP << (pin * 2u));
	else
		//引脚拉低电平
		gpio_ptr->PUPDR |= (uint32_t)(PULL_DOWN << (pin * 2u));
}

//=====================================================================
//函数名称：gpio_enable_int
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//          irqtype：引脚中断类型，由宏定义给出，再次列举如下：
//                  RISING_EDGE  9      //上升沿触发
//                  FALLING_EDGE 10     //下降沿触发
//                  DOUBLE_EDGE  11     //双边沿触发
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时，本函数开启引脚中断，并
//          设置中断触发条件。
//注          意： KL25芯片，只有PORTA、PORTD口具有GPIO类中断功能
//         KW01芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//         KL36芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        STM32L433芯片，PORTA、PORTB、PORTC、PORTH口具有GPIO类中断功能,在EXIT(扩展
//                     中断)处配置，且只包含上升沿触发、下降沿触发、双边沿触发三种引脚中断类型
//=====================================================================
void gpio_enable_int(uint16_t port_pin,uint8_t irqtype)
{
	uint8_t port,pin;    //声明端口port、引脚pin变量
	uint32_t temp;       //临时存放寄存器里的值
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);

	//使能SYSCFG时钟
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//配置对应引脚的扩展中断
	temp = SYSCFG->EXTICR[pin >> 2u];
	temp &= ~(0x0FuL << (4u * (pin & 0x03u)));
	temp |= (port << (4u * (pin & 0x03u)));
	SYSCFG->EXTICR[pin >> 2u] = temp;

	//允许对应扩展中断行的请求
	EXTI->IMR1 |= (uint32_t)(1u<<pin);

	//若上升沿或双边沿触发,使能输入线上升沿触发
	if(RISING_EDGE == irqtype || DOUBLE_EDGE == irqtype)
		EXTI->RTSR1 |= (uint32_t)(1u<<pin);

	//若下降沿或双边沿触发,使能输入线下降沿触发
	if(FALLING_EDGE == irqtype || DOUBLE_EDGE == irqtype)
		EXTI->FTSR1 |= (uint32_t)(1u<<pin);

	if(pin >= 0 && pin <= 4)
		//开中断控制器IRQ中断(EXTIx,x=0-4)
		NVIC_EnableIRQ(table_irq_exti[pin]);
	else if(pin >= 5 && pin <= 9)
		//开中断控制器IRQ中断(EXTI9_5)
		NVIC_EnableIRQ(table_irq_exti[5]);
	else
		//开中断控制器IRQ中断(EXTI15_10)
		NVIC_EnableIRQ(table_irq_exti[6]);
}

//=====================================================================
//函数名称：gpio_disable_int
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时，本函数关闭引脚中断
//注          意： KL25芯片，只有PORTA、PORTD口具有GPIO类中断功能
//         KW01芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//         KL36芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        STM32L433芯片，PORTA、PORTB、PORTC、PORTH口具有GPIO类中断功能,在EXIT(扩展
//                     中断)处配置，且只包含上升沿触发、下降沿触发、双边沿触发三种引脚中断类型
//=====================================================================
void gpio_disable_int(uint16_t port_pin)
{
	uint8_t port,pin;    //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);

	//禁止SYSCFG时钟
	RCC->APB2ENR &= ~RCC_APB2ENR_SYSCFGEN;

	//清对应引脚的扩展中断
	SYSCFG->EXTICR[pin >> 2u] &= ~(0x0FuL << (4u * (pin & 0x03u)));

	//屏蔽对应扩展中断行的请求
	EXTI->IMR1 &= (uint32_t)~(1u<<pin);

	//禁止上升沿下降沿触发
	EXTI->RTSR1 &= (uint32_t)~(1u<<pin);
	EXTI->FTSR1 &= (uint32_t)~(1u<<pin);

	if(pin >= 0 && pin <= 4)
		//开中断控制器IRQ中断(EXTIx,x=0-4)
		NVIC_DisableIRQ(table_irq_exti[pin]);
	else if(pin >= 5 && pin <= 9)
		//开中断控制器IRQ中断(EXTI9_5)
		NVIC_DisableIRQ(table_irq_exti[5]);
	else
		//开中断控制器IRQ中断(EXTI15_10)
		NVIC_DisableIRQ(table_irq_exti[6]);

}

//=====================================================================
//函数名称：gpio_drive_strength
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//       control：控制引脚的驱动能力，LOW_SPEED=低速,MSDIUM_SPEED=中速
//               HIGH_SPEED=高速,VERY_HIGH_SPEED=超高速
//功能概要：（引脚驱动能力：指引脚输入或输出电流的承受力，一般用mA单位度量
//        ，正常驱动能力5mA,高驱动能力18mA。）当引脚被配置为数字输出时,
//        对引脚的驱动能力进行设置
//=====================================================================
void gpio_drive_strength(uint16_t port_pin, uint8_t control)
{
	GPIO_TypeDef *gpio_ptr;    //声明gpio_ptr为GPIO结构体类型指针
	uint8_t port,pin;    //声明端口port、引脚pin变量
//	uint32_t temp;       //临时存放寄存器里的值
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);
	//根据port，给局部变量gpio_ptr赋值（GPIO基地址）
	if(7 == port) //GPIOH
		gpio_ptr = GPIO_ARR[port-2];
	else
		gpio_ptr = GPIO_ARR[port];

	//配置引脚的驱动能力
	gpio_ptr->OSPEEDR |= (uint32_t)(control << (pin * 2u));
}

//=====================================================================
//函数名称：gpio_get_int
//函数返回：引脚GPIO中断标志（1或0）1表示引脚有GPIO中断，0表示引脚没有GPIO中断。
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时,获取中断标志。
//注          意： KL25芯片，只有PORTA、PORTD口具有GPIO类中断功能
//        KW01芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        KL36芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        STM32L433芯片，PORTA、PORTB、PORTC、PORTH口具有GPIO类中断功能,在EXIT(扩展
//                     中断)处配置，且只包含上升沿触发、下降沿触发、双边沿触发三种引脚中断类型
//=====================================================================
uint8_t gpio_get_int(uint16_t port_pin)
{
	uint8_t port,pin;    //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);

	//获取GPIO中断标志
	if((EXTI->PR1 & (1u << pin)) == (1u << pin))
		return 1;
	else
		return 0;
}

//=====================================================================
//函数名称：gpio_clear_int
//函数返回：无
//参数说明：port_pin：(端口号)|(引脚号)（如：(PTB_NUM)|(9) 表示为B口9号脚）
//功能概要：当指定端口引脚被定义为GPIO功能且为输入时,清除中断标志。
//注          意： KL25芯片，只有PORTA、PORTD口具有GPIO类中断功能
//         KW01芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//         KL36芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        STM32L433芯片，PORTA、PORTB、PORTC、PORTH口具有GPIO类中断功能,在EXIT(扩展
//                     中断)处配置，且只包含上升沿触发、下降沿触发、双边沿触发三种引脚中断类型
//=====================================================================
void gpio_clear_int(uint16_t port_pin)
{
	uint8_t port,pin;    //声明端口port、引脚pin变量
	//根据带入参数port_pin，解析出端口与引脚分别赋给port,pin
	gpio_get_port_pin(port_pin,&port,&pin);

	//清GPIO中断标志
	EXTI->PR1 |= (1u << pin);
}

//=====================================================================
//函数名称：gpio_clear_allint
//函数返回：无
//参数说明：无
//功能概要：清除所有端口的GPIO中断
//注          意： KL25芯片，只有PORTA、PORTD口具有GPIO类中断功能
//         KW01芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//         KL36芯片，只有PORTA、PORTC、PORTD口具有GPIO类中断功能
//        STM32L433芯片，PORTA、PORTB、PORTC、PORTH口具有GPIO类中断功能,在EXIT(扩展
//                     中断)处配置，且只包含上升沿触发、下降沿触发、双边沿触发三种引脚中断类型
//=====================================================================
void gpio_clear_allint(void)
{
	EXTI->PR1 |= 0xFFFF;//pin可取0-15
}


//----------------------以下为内部函数存放处-----------------
//=====================================================================
//函数名称：gpio_get_port_pin
//函数返回：无
//参数说明：port_pin：端口号|引脚号（如：(PTB_NUM)|(9) 表示为B口9号脚）
//       port：端口号（传指带出参数）
//       pin:引脚号（0~15，实际取值由芯片的物理引脚决定）（传指带出参数）
//功能概要：将传进参数port_pin进行解析，得出具体端口号与引脚号，分别赋值给port与pin,返回。
//       （例：(PTB_NUM)|(9)解析为PORTB与9，并将其分别赋值给port与pin）。
//=====================================================================
void gpio_get_port_pin(uint16_t port_pin,uint8_t* port,uint8_t* pin)
{
	*port = (port_pin>>8);
	*pin = port_pin;
}
