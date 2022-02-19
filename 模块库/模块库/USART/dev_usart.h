/*******************************************************************************
 *   Copyright (C)  muyuan Co., Ltd.
 *				  All Rights Reserved.
 *   Department: R&D SW
 ********************************************************************************
 * Author      : 康国伟
 * Version     : v0.1
 * Date        : 2021/06/10
 * Description : 这里对接软件HAL库串口,给API通用串口函数提供可能
 *******************************************************************************/

#ifndef __UART__CONFIG_H__
#define __UART__CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "dev_cfg.h"


/********************config_define_start *******************************/
#define IS_USING_USART1  			(1)	/* 如果使用串口1 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_USART2  			(0)	/* 如果使用串口2 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_USART3  			(0)	/* 如果使用串口3 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_UART4  			(0)	/* 如果使用串口4 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_UART5 				(0)	/* 如果使用串口5 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_USART6				(0)	/* 如果使用串口6 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_UART7  			(0)	/* 如果使用串口7 除了配置串口中断+DMA外 请激活我 */
#define IS_USING_UART8 				(0)	/* 如果使用串口8 除了配置串口中断+DMA外 请激活我 */
#define UART_BUFF_LENGTH  			(256)	/* 如果串口接收的是GPS数据，则需要将这个缓存区扩大到512+ */

/********************config_define_end *********************************/

/********************public_define_start *******************************/
#if IS_USING_USART1
#define UART1_HANDLE	huart1
#define HDMA_UART1_RX	hdma_usart1_rx
#endif

#if IS_USING_USART2
#define UART2_HANDLE	huart2
#define HDMA_UART2_RX	hdma_usart2_rx
#endif

#if IS_USING_USART3
#define UART3_HANDLE	huart3
#define HDMA_UART3_RX	hdma_usart3_rx
#endif

#if IS_USING_UART4
#define UART4_HANDLE	huart4
#define HDMA_UART4_RX	hdma_uart4_rx
#endif

#if IS_USING_UART5
#define UART5_HANDLE	huart5
#define HDMA_UART5_RX	hdma_uart5_rx
#endif

#if IS_USING_USART6
#define UART6_HANDLE	huart6
#define HDMA_UART6_RX	hdma_usart6_rx
#endif

#if IS_USING_UART7
#define UART7_HANDLE	huart7
#define HDMA_UART7_RX	hdma_uart7_rx
#endif

#if IS_USING_UART8
#define UART8_HANDLE	huart8
#define HDMA_UART8_RX	hdma_uart8_rx
#endif
/********************public_define_end *********************************/


/********************custom_type_start *********************************/
#if IS_USING_USART
typedef struct
{
	uint16_t length;
	uint8_t rxbuff[UART_BUFF_LENGTH];
}T_UARTRMSG;
#endif

typedef enum
{
#if IS_USING_USART1
	USART_NUM_1,
#endif
#if IS_USING_USART2
	USART_NUM_2,
#endif
#if IS_USING_USART3
	USART_NUM_3,
#endif
#if IS_USING_UART4
	USART_NUM_4,
#endif
#if IS_USING_UART5
	USART_NUM_5,
#endif
#if IS_USING_USART6
	USART_NUM_6,
#endif
#if IS_USING_UART7
	USART_NUM_7,
#endif
#if IS_USING_UART8
	USART_NUM_8,
#endif
}USART_NUM;
/********************custom_type_end ************************************/


/********************begin_parm *****************************************/
/********************end_parm *******************************************/

/********************public_function_start ******************************/
extern void dev_usartInit(void);								//放到主函数
extern void USART_RxIdleCallback(UART_HandleTypeDef *huart); 	//放到串口中断函数里
extern void dev_uartSend(USART_NUM num, uint8_t *p_sendBuf, uint16_t sendLen);	//通用串口API



/* 下面这些函数不用高配置，系统会主动调用*/
extern void uart1_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart2_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart3_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart4_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart5_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart6_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart7_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
extern void uart8_rxcallback(uint8_t *rxdate, uint16_t len);	//归USART_RxIdleCallback调用
/********************public_function_send********************************/



#endif
