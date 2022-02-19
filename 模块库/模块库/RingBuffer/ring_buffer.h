/*******************************************************************************
 *   Copyright (C)  MuYuan Co., Ltd.
 *				  All Rights Reserved.
 *   Department: R&D SW
 ********************************************************************************
 * ��   ��: 		KangSir
 * �� �� ��:  	v1.0
 * ��������:   	2021/12/21
 * ����ģ��:		ͨ���㷨��(���Ա��ײ��Ӧ�ò�ʹ��)
 * Description : ���ͻ�����
 * �ο�����:  https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device-ipc/ringbuffer/ringbuffer?id=%e7%ae%80%e4%bb%8b
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
/** ����������ʣ��ռ� */
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
 * Description	: ����������̬��ʼ������(�ڶ���)
 * Input:
 *    uint16_t size	: ���λ���������ռ��С(��λ:�ֽ�)
 * Return	rt_ringbuffer*: �ɹ����ؽṹ��ָ��,ʧ�ܷ���NULL
 */
extern struct rt_ringbuffer* rt_ringbuffer_create(uint16_t size);

/*
 * Description	: ���ٴ����Ļ�����
 * Input:
 *    struct rt_ringbuffer *rb: ��Ҫ���ٵ�ringbuffer�ṹ��ָ��
 * Return	: void
 */
extern void rt_ringbuffer_destroy(struct rt_ringbuffer *rb);


/*
 * Description	: ����������̬��ʼ������(��ջ��)
 * Input:
 *    struct rt_ringbuffer *rb		: AIֵ����
 *    FLOAT		: AIֵ
 * Return		: �ɹ�����TRUE,���򷵻�FALSE
 */
extern void rt_ringbuffer_init(struct rt_ringbuffer *rb, uint8_t *pool, int16_t size);

/*
 * Description	: ��λ�����Ļ�����
 * Input:
 *    struct rt_ringbuffer *rb: ��Ҫ�����ringbuffer�ṹ��ָ��
 * Return	: void
 */
extern void rt_ringbuffer_reset(struct rt_ringbuffer *rb);


/*
 * Description	: ���λ�����д��һ���ֽ�
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫд���ringbuffer�ṹ��ָ��
 *    const uint8_t ch			: Ҫд����ֽ�
 * Return	uint32_t			: 1: д��ɹ�  0: д��ʧ��(�����������д��ʧ��)
 */
extern uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const uint8_t ch);


/*
 * Description	: ���λ�����ǿ��д��һ���ֽ�(�����������������Ḳ����ɵ��Ǹ��ֽ�)
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫд���ringbuffer�ṹ��ָ��
 *    const uint8_t ch			: Ҫд����ֽ�
 * Return	uint32_t			: 1: д��ɹ�(����д��ʧ�ܵ�)
 */
extern uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const uint8_t ch);


/*
 * Description	: ���λ�����д��һ������
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫд���ringbuffer�ṹ��ָ��
 *    const uint8_t *ptr		: Ҫд��һ������
 *    uint16_t length			: д��������ֽ���
 * Return	uint32_t			: ��������д����ֽ���(������ÿռ���10,�����д��������ֽ�����20,�򷵻�10)
 */
extern uint32_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);


/*
 * Description	: ���λ�����ǿ��д��һ������
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫд���ringbuffer�ṹ��ָ��
 *    const uint8_t *ptr		: Ҫд��һ������
 *    uint16_t length			: д��������ֽ���
 * Return	uint32_t			: ��������д����ֽ���(����ʧ�ܣ�ֻ�����Ḳ�Ƕ���)
 */
extern uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);

/*
 * Description	: �ӻ��λ�������ȡһ������
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫ��ȡ��ringbuffer�ṹ��ָ��
 *    const uint8_t *ptr		: ��ȡ��һ���ֽڵ�����
 * Return	uint32_t			: ���������������ֽ���(1:�ɹ�  0:ʧ��)
 */
extern uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch);

/*
 * Description	: �ӻ��λ�������ȡһ������
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫ��ȡ��ringbuffer�ṹ��ָ��
 *    uint8_t *ptr				: ��ȡ��һ�����ݷ��õĻ�����
 *    uint16_t length			: �ƻ�Ҫ��ȡ���ֽ���
 * Return	uint32_t			: ���������������ֽ���
 */
extern uint32_t rt_ringbuffer_get(struct rt_ringbuffer *rb, uint8_t *ptr, uint16_t length);

/*
 * Description	: ��ȡringbuffer���Ѵ洢�����ݴ�С
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫ��ȡ��ringbuffer�ṹ��ָ��
 * Return	uint32_t			: �������洢���ֽ���
 */
extern uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb);


/*
 * Description	: ��ȡringbuffer�ڲ��������ܴ�С��
 * Input:
 *    struct rt_ringbuffer *rb	: Ҫ��ȡ��ringbuffer�ṹ��ָ��
 * Return	uint32_t			: �ڲ����������ܴ�С(���ֽ�Ϊ��λ)
 */
extern uint16_t rt_ringbuffer_get_size(struct rt_ringbuffer *rb);


/*
 * Description	: �����õĳ����
 */
extern void ringBufferTest(void);
/********************public_function_send********************************/

#ifdef __cplusplus
}
#endif

#endif
