/*******************************************************************************
 *   Copyright (C)  kangsir Co., Ltd.
 *				  All Rights Reserved.
 *   Department: R&D SW
 ********************************************************************************
 * Author      : 康国伟
 * Version     : v0.1
 * Date        : 2021/12/20
 * Description : 这里提供一些项目中常用的操作
 *******************************************************************************/

#ifndef __ALGORIGHM__API_H__
#define __ALGORIGHM__API_H__

#include "main.h"
#include "stdio.h"

/********************Basic_Config_Start*****************************************/
#define IS_USING_BLOCK_DELAY			(1)		/* 选择是否使用阻塞延迟 */

#define PRINT_SWITCH					(1)			/* 如果这个开关不打开，则不打印*/
#define PRINTF_DEBUG_INFO 				(0xAA)		/* 打印调试信息标志(调试代码时有用) */
#define PRINTF_ERROR_INFO 				(0xBB)		/* 打印错误信息标志(调试代码时有用) */
/********************Basic_Config_End********************************************/

/********************begin define  *****************************************/
#define SET_BIT_NUM(m, n) ((m) |= (1 << (n)))		   /* 数字m的第n位置1 */
#define CLR_BIT_NUM(m, n) ((m) &= (~(1 << (n))))	   /* 数字m的第n位清0 */
#define GET_BIT_NUM(m, n) (((m) & (1 << (n))) ? 1 : 0) /* 获取数字m的第n位状态 */

#define GET_MIN_NUM(m, n) (((m) > (n)) ? (n) : (m)) /* 两者较小 */
#define GET_MAX_NUM(m, n) (((m) > (n)) ? (m) : (n)) /* 两者较大 */
#define MY_UNUSED(V) ((void *)(V))					/* 消除警告用的 */

/* 基于某种类型在指定地址写指定数据 */
#define WRITE_TO_ADDRESS(type, address, value) *((type *)(address)) = value;

/* 基于某种类型从指定地址读取数据 */
#define READ_FROM_ADDRESS(type, address) *((type *)(address));

/* 向上取整，确保数据按照align的值对齐 */
#define MY_ALIGN_SIZE(size, align) (((size) + align - 1U) & ~(align - 1U))

/* 获取数组元素个数(用过的自然懂) */
#define GET_ARRAY_LENGTH(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

/* 非阻塞延迟，第一次调用此函数则不会return */
#define MY_Delay(timer)                      \
		do                                       \
		{                                        \
			static uint32_t timeOut = 0;         \
			if (HAL_GetTick() - timeOut < timer) \
			return;                          \
			timeOut = HAL_GetTick();             \
		} while (0)

/*
 * PT_CONTENT: 打印内容
 * PT_WAY	 : 打印方式
 * 举例: 假如工程处于研发阶段，需要时不时地打印一些东西
 * 如果这些东西假如是错误信息则(PT_WAY取值为PRINTF_ERROR_INFO)
 * 如果这些东西假如是错误信息则(PT_WAY取值为PRINTF_DEBUG_INFO)
 * 如果工程已经趋于稳定了，不想让串口打印那么多影响到设备性能，则将PRINT_SWITCH 置0
 * 温馨提示: printf需结合硬件的情况选择响应的串口
 */
#define MY_PRINTF(PT_CONTENT, PT_WAY)                               	\
		do                                                          	\
		{                                                           	\
			if(PRINT_SWITCH)break;										\
			if (PRINTF_ERROR_INFO == PT_WAY)                      		\
			{                                                       	\
				printf("[error]:\t[%s]\t failed at line %d in %s\r\n", 	\
						PT_CONTENT, __LINE__, __FILE__);         		\
						printf("[error]:\t %s\r\n", PT_CONTENT);		\
			}                      			                       		\
			else if(PRINTF_DEBUG_INFO == PT_WAY)        				\
			{                                                       	\
				printf("[debug]:\t %s\r\n", PT_CONTENT);				\
			}                                                       	\
		} while (0);
/**********************end_define *****************************************/

/********************begin_type *****************************************/

/*********************end_type *****************************************/

/********************begin_parm *****************************************/
/*********************end_parm *****************************************/

/********************begin_func *****************************************/

extern int32_t PRINT_DEBUG(char *, ...);	//打印调试信息

/* modbus 里crc校验用的着 */
extern uint16_t get_crc_result(uint8_t *pucFrame, uint16_t usLen);
extern uint32_t str_to_hex(char *p_str); /* 字符串转16进制 */
extern uint32_t str_to_num(char *p_str); /* 字符串转10进制 */

#if IS_USING_BLOCK_DELAY
/* 阻塞类延迟, 在线程中不要轻易使用哈，主要是给类似于软件模拟IIC时序用的 */
extern void delay_init(uint8_t sysclk);
extern void delay_us(uint32_t nus);		/* 不能超过22.5ms*/
extern void delay_ms(uint16_t nms);		/* 没有限制,不过在有操作系统的环境下不推荐使用 */
#endif
/*********************end_func *****************************************/
#endif
