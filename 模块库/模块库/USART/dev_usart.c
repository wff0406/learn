/********************************************************
 *  Created on: 2020年06月10日
 *  Author: KangSir
 *********************************************************/

#include "dev_usart.h"
#include "string.h"
#include "stdio.h"



/********************private_define_start ****************************************/

#define UART_RECEICE_ASK(huart)														\
		do{																			\
			uint8_t uart_buf[UART_BUFF_LENGTH] = {0};								\
			memset(uart_buf,0,sizeof(uart_buf));									\
			sprintf((char*)uart_buf, "buffer[%d] = \"%s\"\r\n", len, (char*)rxdate);\
			HAL_UART_Transmit(&huart, (uint8_t *)uart_buf, strlen((const char *)uart_buf), 0x1FF);\
		}while(0);
/********************private_define_end ******************************************/


/********************static_global_variable_start ********************************/

//public:外部已定义好的的变量
#if IS_USING_USART1
static T_UARTRMSG sg_uart1_receive_msg;
extern DMA_HandleTypeDef HDMA_UART1_RX;
#endif

#if IS_USING_USART2
static T_UARTRMSG sg_uart2_receive_msg;
extern DMA_HandleTypeDef HDMA_UART2_RX;
#endif

#if IS_USING_USART3
static T_UARTRMSG sg_uart3_receive_msg;
extern DMA_HandleTypeDef HDMA_UART3_RX;
#endif

#if IS_USING_UART4
static T_UARTRMSG sg_uart4_receive_msg;
extern DMA_HandleTypeDef HDMA_UART4_RX;
#endif

#if IS_USING_UART5
static T_UARTRMSG sg_uart5_receive_msg;
extern DMA_HandleTypeDef HDMA_UART5_RX;
#endif

#if IS_USING_USART6
static T_UARTRMSG sg_uart6_receive_msg;
extern DMA_HandleTypeDef HDMA_UART6_RX;
#endif

#if IS_USING_UART7
static T_UARTRMSG sg_uart7_receive_msg;
extern DMA_HandleTypeDef HDMA_UART7_RX;
#endif

#if IS_USING_UART8
static T_UARTRMSG sg_uart8_receive_msg;
extern DMA_HandleTypeDef HDMA_UART8_RX;
#endif
/********************static_global_variable_end **********************************/


/********************debug_variable_start *****************************************/
/********************debug_variable_end  ******************************************/

/********************static_function_start*****************************************/
/********************static_function_end ******************************************/


/********************extern_function_start ****************************************/

/*-------------------------------------------------------------------
 * @brief:		串口配置函数(向下与HAL库对接)
 * 				可通过串口宏(IS_USING_USART1...)配合CubeMX配合激活某个串口的功能
 * @input:		void
 * @return:		void
----------------------------------------------------------------------*/
void dev_usartInit(void)
{
#if IS_USING_USART
	T_UARTRMSG *uartrx;
#endif

#if IS_USING_USART1
	uartrx = &sg_uart1_receive_msg;
	__HAL_UART_ENABLE_IT(&UART1_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART1_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_USART2
	uartrx = &sg_uart2_receive_msg;
	__HAL_UART_ENABLE_IT(&UART2_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART2_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_USART3
	uartrx = &sg_uart3_receive_msg;
	__HAL_UART_ENABLE_IT(&UART3_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART3_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_UART4
	uartrx = &sg_uart4_receive_msg;
	__HAL_UART_ENABLE_IT(&UART4_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART4_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_UART5
	uartrx = &sg_uart5_receive_msg;
	__HAL_UART_ENABLE_IT(&UART5_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART5_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_USART6
	uartrx = &sg_uart6_receive_msg;
	__HAL_UART_ENABLE_IT(&UART6_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART6_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_UART7
	uartrx = &sg_uart7_receive_msg;
	__HAL_UART_ENABLE_IT(&UART7_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART7_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif

#if IS_USING_UART8
	uartrx = &sg_uart8_receive_msg;
	__HAL_UART_ENABLE_IT(&UART8_HANDLE, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&UART8_HANDLE, uartrx->rxbuff, sizeof(uartrx->rxbuff));
#endif
}

void USART_RxIdleCallback(UART_HandleTypeDef *huart)
{
#if IS_USING_USART
	T_UARTRMSG *uartrx;
#endif

#if IS_USING_USART1
	if (huart == &UART1_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart1_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART1_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART1_HANDLE, UART_IT_IDLE);
			uart1_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART1_HANDLE, UART_IT_IDLE);
		}
	}
#endif

#if IS_USING_USART2
	if (huart == &UART2_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart2_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART2_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART2_HANDLE, UART_IT_IDLE);
			uart2_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART2_HANDLE, UART_IT_IDLE);
		}
	}
#endif

#if IS_USING_USART3
	if (huart == &UART3_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart3_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART3_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART3_HANDLE, UART_IT_IDLE);
			uart3_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART3_HANDLE, UART_IT_IDLE);
		}
	}
#endif

#if IS_USING_UART4
	if (huart == &UART4_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart4_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);

			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART4_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART4_HANDLE, UART_IT_IDLE);
			uart4_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART4_HANDLE, UART_IT_IDLE);
		}
	}
#endif
#if IS_USING_UART5
	if (huart == &UART5_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart5_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);

			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART5_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART5_HANDLE, UART_IT_IDLE);
			uart5_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART5_HANDLE, UART_IT_IDLE);
		}
	}
#endif
#if IS_USING_USART6
	if (huart == &UART6_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart6_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART6_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART6_HANDLE, UART_IT_IDLE);
			uart6_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART6_HANDLE, UART_IT_IDLE);
		}
	}
#endif

#if IS_USING_UART7
	if (huart == &UART7_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart7_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART7_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART7_HANDLE, UART_IT_IDLE);
			uart7_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART7_HANDLE, UART_IT_IDLE);
		}
	}
#endif

#if IS_USING_UART8
	if (huart == &UART8_HANDLE)
	{
		if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
		{
			uartrx = &sg_uart8_receive_msg;
			HAL_UART_AbortReceive(huart);
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uartrx->length = sizeof(uartrx->rxbuff) - __HAL_DMA_GET_COUNTER(&HDMA_UART8_RX);
			HAL_UART_Receive_DMA(huart, uartrx->rxbuff, sizeof(uartrx->rxbuff));

			__HAL_UART_DISABLE_IT(&UART8_HANDLE, UART_IT_IDLE);
			uart8_rxcallback(uartrx->rxbuff, uartrx->length);
			__HAL_UART_ENABLE_IT(&UART8_HANDLE, UART_IT_IDLE);
		}
	}
#endif
}


/*-------------------------------------------------------------------
 * @brief:		串口通用发送函数(为上层应用提供统一接口)
 * @serial_num:	所使用的的串口号(1:表示使用串口1发送数据)
 * @p_str:		发送的数据缓存区地址
 * @len:		发送数据字节数
 * @return:		void
-------------------------------------------------------------------*/
void dev_uartSend(USART_NUM num, uint8_t *p_sendBuf, uint16_t sendLen)
{
#if IS_USING_USART
	if(0 == sendLen)return;
#endif
#if IS_USING_USART1
	if(USART_NUM_1 == num)
	{
		HAL_UART_Transmit(&UART1_HANDLE, (uint8_t *)p_sendBuf, sendLen, 0x1FF);
	}
#endif

#if IS_USING_USART2
	if(USART_NUM_2 == num)
	{
		HAL_UART_Transmit(&UART2_HANDLE, (uint8_t *)p_sendBuf, len, 0x1FF);
	}
#endif

#if IS_USING_USART3
	if(USART_NUM_3 == num)
	{
		//如果是485的话可参考这个例子哈
		HAL_GPIO_WritePin(RS485_CK_GPIO_Port, RS485_CK_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&UART3_HANDLE, p_sendBuf, sendLen, 0x1FF);
		HAL_GPIO_WritePin(RS485_CK_GPIO_Port, RS485_CK_Pin, GPIO_PIN_RESET);
		return;
	}
#endif

#if IS_USING_UART4
	if(USART_NUM_4 == num)
	{
		HAL_UART_Transmit(&UART4_HANDLE, (uint8_t *)p_sendBuf, sendLen, 0x1FF);
	}
#endif

#if IS_USING_UART5
	if(USART_NUM_5 == num)
	{
		HAL_UART_Transmit(&UART5_HANDLE, (uint8_t *)p_sendBuf, sendLen, 0x1FF);
	}
#endif

#if IS_USING_USART6
	if(USART_NUM_6 == num)
	{
		HAL_UART_Transmit(&UART6_HANDLE, p_sendBuf, sendLen, 0x1FF);
		return;
	}
#endif

#if IS_USING_UART7
	if(USART_NUM_7 == num)
	{
		HAL_UART_Transmit(&UART7_HANDLE, p_str, len, 0x1FF);
		return;
	}
#endif

#if IS_USING_UART8
	if(USART_NUM_8 == num)
	{
		HAL_UART_Transmit(&UART8_HANDLE, p_str, len, 0x1FF);
		return;
	}
#endif
}


#if IS_USING_USART1
__weak void uart1_rxcallback(uint8_t *rxdate, uint16_t len)
{
#if 1
	UART_RECEICE_ASK(UART1_HANDLE);
#endif
}
#endif

#if IS_USING_USART2
__weak void uart2_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART2_HANDLE);
}
#endif

#if IS_USING_USART3
__weak void uart3_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART3_HANDLE);
}
#endif

#if IS_USING_UART4
__weak void uart4_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART4_HANDLE);
}
#endif

#if IS_USING_UART5
__weak void uart5_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART5_HANDLE);
}
#endif

#if IS_USING_USART6
__weak void uart6_rxcallback(uint8_t *rxdate, uint16_t len)
{
#if 0
	/* 串口接手处理函数，这里的例子是ESP32 蓝牙模组 */
	//bt_server_prase(rxdate, len);
#else
	UART_RECEICE_ASK(UART6_HANDLE);
#endif
}
#endif

#if IS_USING_UART7
__weak void uart7_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART7_HANDLE);
}
#endif

#if IS_USING_UART8
__weak void uart8_rxcallback(uint8_t *rxdate, uint16_t len)
{
	UART_RECEICE_ASK(UART8_HANDLE);
}
#endif

/********************extern_function_end  *****************************************/
