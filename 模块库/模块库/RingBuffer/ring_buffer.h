/*******************************************************************************
 *   Copyright (C)  MuYuan Co., Ltd.
 *				  All Rights Reserved.
 *   Department: R&D SW
 ********************************************************************************
 * 作   者: 		KangSir
 * 版 本 号:  	v1.0
 * 生成日期:   	2021/12/21
 * 所述模块:		通用算法层(可以被底层或应用层使用)
 * Description : 环型缓冲区
 * 参考链接:  https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device-ipc/ringbuffer/ringbuffer?id=%e7%ae%80%e4%bb%8b
 *******************************************************************************/

#ifndef __KS_RING_BUFFER_H__
#define __KS_RING_BUFFER_H__

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif



/********************config_define_start *******************************/
/********************config_define_end *********************************/

/********************public_define_start *******************************/
/** 反馈环形区剩余空间 */
#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))
/********************public_define_end *********************************/


/********************custom_type_start *********************************/

struct rt_ringbuffer
{
	uint8_t *buffer_ptr;
	/* use the msb of the {read,write}_index as mirror bit. You can see this as
	 * if the buffer adds a virtual mirror and the pointers point either to the
	 * normal or to the mirrored buffer. If the write_index has the same value
	 * with the read_index, but in a different mirror, the buffer is full.
	 * While if the write_index and the read_index are the same and within the
	 * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
	 *
	 *          mirror = 0                    mirror = 1
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 *  read_idx-^                   write_idx-^
	 *
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * read_idx-^ ^-write_idx
	 *
	 * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
	 * But it should be enough for most of the cases.
	 *
	 * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
	uint16_t read_mirror : 1;
	uint16_t read_index : 15;
	uint16_t write_mirror : 1;
	uint16_t write_index : 15;
	/* as we use msb of index as mirror bit, the size should be signed and
	 * could only be positive. */
	int16_t buffer_size;
};

enum rt_ringbuffer_state
{
	RT_RINGBUFFER_EMPTY,
	RT_RINGBUFFER_FULL,
	/* half full is neither full nor empty */
	RT_RINGBUFFER_HALFFULL,
};

typedef struct rb_example
{
	int a;
	int b;
} rb_example_t;
/********************custom_type_end ************************************/


/********************begin_parm *****************************************/
/********************end_parm *******************************************/


/********************public_function_start ******************************/


/*
 * Description	: 环缓冲区动态初始化函数(在堆中)
 * Input:
 *    uint16_t size	: 环形缓冲区申请空间大小(单位:字节)
 * Return	rt_ringbuffer*: 成功返回结构体指针,失败返回NULL
 */
extern struct rt_ringbuffer* rt_ringbuffer_create(uint16_t size);

/*
 * Description	: 销毁创建的缓存区
 * Input:
 *    struct rt_ringbuffer *rb: 需要销毁的ringbuffer结构体指针
 * Return	: void
 */
extern void rt_ringbuffer_destroy(struct rt_ringbuffer *rb);


/*
 * Description	: 环缓冲区静态初始化函数(在栈中)
 * Input:
 *    struct rt_ringbuffer *rb		: AI值索引
 *    FLOAT		: AI值
 * Return		: 成功返回TRUE,否则返回FALSE
 */
extern void rt_ringbuffer_init(struct rt_ringbuffer *rb, uint8_t *pool, int16_t size);

/*
 * Description	: 复位创建的缓存区
 * Input:
 *    struct rt_ringbuffer *rb: 需要清零的ringbuffer结构体指针
 * Return	: void
 */
extern void rt_ringbuffer_reset(struct rt_ringbuffer *rb);


/*
 * Description	: 向环形缓存区写入一个字节
 * Input:
 *    struct rt_ringbuffer *rb	: 要写入的ringbuffer结构体指针
 *    const uint8_t ch			: 要写入的字节
 * Return	uint32_t			: 1: 写入成功  0: 写入失败(缓存区满后会写入失败)
 */
extern uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const uint8_t ch);


/*
 * Description	: 向环形缓存区强制写入一个字节(如果缓存区已满，则会覆盖最旧的那个字节)
 * Input:
 *    struct rt_ringbuffer *rb	: 要写入的ringbuffer结构体指针
 *    const uint8_t ch			: 要写入的字节
 * Return	uint32_t			: 1: 写入成功(不会写入失败的)
 */
extern uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const uint8_t ch);


/*
 * Description	: 向环形缓存区写入一坨数据
 * Input:
 *    struct rt_ringbuffer *rb	: 要写入的ringbuffer结构体指针
 *    const uint8_t *ptr		: 要写入一串数据
 *    uint16_t length			: 写入的数据字节数
 * Return	uint32_t			: 返回真正写入的字节数(比如可用空间是10,结果让写入的数据字节数是20,则返回10)
 */
extern uint32_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);


/*
 * Description	: 向环形缓存区强制写入一坨数据
 * Input:
 *    struct rt_ringbuffer *rb	: 要写入的ringbuffer结构体指针
 *    const uint8_t *ptr		: 要写入一串数据
 *    uint16_t length			: 写入的数据字节数
 * Return	uint32_t			: 返回真正写入的字节数(不会失败，只不过会覆盖而已)
 */
extern uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);

/*
 * Description	: 从环形缓存区读取一个数据
 * Input:
 *    struct rt_ringbuffer *rb	: 要读取的ringbuffer结构体指针
 *    const uint8_t *ptr		: 获取的一个字节的数据
 * Return	uint32_t			: 返回真正读到的字节数(1:成功  0:失败)
 */
extern uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch);

/*
 * Description	: 从环形缓存区读取一个数据
 * Input:
 *    struct rt_ringbuffer *rb	: 要读取的ringbuffer结构体指针
 *    uint8_t *ptr				: 读取的一坨数据放置的缓存区
 *    uint16_t length			: 计划要读取的字节数
 * Return	uint32_t			: 返回真正读到的字节数
 */
extern uint32_t rt_ringbuffer_get(struct rt_ringbuffer *rb, uint8_t *ptr, uint16_t length);

/*
 * Description	: 获取ringbuffer内已存储的数据大小
 * Input:
 *    struct rt_ringbuffer *rb	: 要读取的ringbuffer结构体指针
 * Return	uint32_t			: 环形区存储的字节数
 */
extern uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb);


/*
 * Description	: 获取ringbuffer内部缓冲区总大小：
 * Input:
 *    struct rt_ringbuffer *rb	: 要读取的ringbuffer结构体指针
 * Return	uint32_t			: 内部缓冲区的总大小(以字节为单位)
 */
extern uint16_t rt_ringbuffer_get_size(struct rt_ringbuffer *rb);


/*
 * Description	: 测试用的程序哈
 */
extern void ringBufferTest(void);
/********************public_function_send********************************/

#ifdef __cplusplus
}
#endif

#endif
