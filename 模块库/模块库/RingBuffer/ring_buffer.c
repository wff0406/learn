/********************************************************************************
 *   Copyright (C) 	MuYuan Company, Ltd.
 *					All Rights Reserved.
 *   Department: R&D SW
 *   Author: KangGuoWei
 *********************************************************************************/
#include "ring_buffer.h"
#include <string.h>
#include <stdlib.h>

/*===========================Date:Name:Changes====================================
2021.12.21:	康国伟
环缓冲初级版本，参考RT-Thread代码，具体说明参考如下链接:
https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device-ipc/ringbuffer/ringbuffer?id=%e7%ae%80%e4%bb%8b
 *================================================================================*/

#include "main.h"
#include "ring_buffer.h"

/********************private_define_start ****************************************/
#define RT_ALIGN_SIZE					4
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
/********************private_define_end ******************************************/


/********************static_global_variable_start ********************************/
/********************static_global_variable_end **********************************/


/********************debug_variable_start *****************************************/
/********************debug_variable_end  ******************************************/

/********************static_function_start*****************************************/
static inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
	if (rb->read_index == rb->write_index)
	{
		if (rb->read_mirror == rb->write_mirror)
			return RT_RINGBUFFER_EMPTY;
		else
			return RT_RINGBUFFER_FULL;
	}
	return RT_RINGBUFFER_HALFFULL;
}
/********************static_function_end ******************************************/


/********************extern_function_start ****************************************/

void rt_ringbuffer_init(struct rt_ringbuffer *rb,
		uint8_t           *pool,
		int16_t            size)
{
	if(size <= 0)return;

	/* initialize read and write index */
	rb->read_mirror = rb->read_index = 0;
	rb->write_mirror = rb->write_index = 0;

	/* set buffer pool and size */
	rb->buffer_ptr = pool;
	rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}


/**
 * put a block of data into ring buffer
 */
uint32_t rt_ringbuffer_put(struct rt_ringbuffer *rb,
		const uint8_t     *ptr,
		uint16_t           length)
{
	uint16_t size;
	if(NULL == rb)return 0;

	/* whether has enough space */
	size = rt_ringbuffer_space_len(rb);

	/* no space */
	if (size == 0)
		return 0;

	/* drop some data */
	if (size < length)
		length = size;

	if (rb->buffer_size - rb->write_index > length)
	{
		/* read_index - write_index = empty space */
		memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->write_index += length;
		return length;
	}

	memcpy(&rb->buffer_ptr[rb->write_index],
			&ptr[0],
			rb->buffer_size - rb->write_index);
	memcpy(&rb->buffer_ptr[0],
			&ptr[rb->buffer_size - rb->write_index],
			length - (rb->buffer_size - rb->write_index));

	/* we are going into the other side of the mirror */
	rb->write_mirror = ~rb->write_mirror;
	rb->write_index = length - (rb->buffer_size - rb->write_index);

	return length;
}


/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb,
		const uint8_t     *ptr,
		uint16_t           length)
{
	uint16_t space_length;

	if(NULL == rb)return 0;

	space_length = rt_ringbuffer_space_len(rb);

	if (length > rb->buffer_size)
	{
		ptr = &ptr[length - rb->buffer_size];
		length = rb->buffer_size;
	}

	if (rb->buffer_size - rb->write_index > length)
	{
		/* read_index - write_index = empty space */
		memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->write_index += length;

		if (length > space_length)
			rb->read_index = rb->write_index;

		return length;
	}

	memcpy(&rb->buffer_ptr[rb->write_index],
			&ptr[0],
			rb->buffer_size - rb->write_index);
	memcpy(&rb->buffer_ptr[0],
			&ptr[rb->buffer_size - rb->write_index],
			length - (rb->buffer_size - rb->write_index));

	/* we are going into the other side of the mirror */
	rb->write_mirror = ~rb->write_mirror;
	rb->write_index = length - (rb->buffer_size - rb->write_index);

	if (length > space_length)
	{
		rb->read_mirror = ~rb->read_mirror;
		rb->read_index = rb->write_index;
	}

	return length;
}

/**
 *  get data from ring buffer
 */
uint32_t rt_ringbuffer_get(struct rt_ringbuffer *rb,
		uint8_t           *ptr,
		uint16_t           length)
{
	uint32_t size;

	if(NULL == rb)return 0;

	/* whether has enough data  */
	size = rt_ringbuffer_data_len(rb);

	/* no data */
	if (size == 0)
		return 0;

	/* less data */
	if (size < length)
		length = size;

	if (rb->buffer_size - rb->read_index > length)
	{
		/* copy all of data */
		memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
		/* this should not cause overflow because there is enough space for
		 * length of data in current mirror */
		rb->read_index += length;
		return length;
	}

	memcpy(&ptr[0],
			&rb->buffer_ptr[rb->read_index],
			rb->buffer_size - rb->read_index);
	memcpy(&ptr[rb->buffer_size - rb->read_index],
			&rb->buffer_ptr[0],
			length - (rb->buffer_size - rb->read_index));

	/* we are going into the other side of the mirror */
	rb->read_mirror = ~rb->read_mirror;
	rb->read_index = length - (rb->buffer_size - rb->read_index);

	return length;
}

/**
 * put a character into ring buffer
 */
uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const uint8_t ch)
{
	if(NULL == rb)return 0;

	/* whether has enough space */
	if (!rt_ringbuffer_space_len(rb))
		return 0;

	rb->buffer_ptr[rb->write_index] = ch;

	/* flip mirror */
	if (rb->write_index == rb->buffer_size-1)
	{
		rb->write_mirror = ~rb->write_mirror;
		rb->write_index = 0;
	}
	else
	{
		rb->write_index++;
	}

	return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const uint8_t ch)
{
	enum rt_ringbuffer_state old_state;

	if(NULL == rb)return 0;

	old_state = rt_ringbuffer_status(rb);

	rb->buffer_ptr[rb->write_index] = ch;

	/* flip mirror */
	if (rb->write_index == rb->buffer_size-1)
	{
		rb->write_mirror = ~rb->write_mirror;
		rb->write_index = 0;
		if (old_state == RT_RINGBUFFER_FULL)
		{
			rb->read_mirror = ~rb->read_mirror;
			rb->read_index = rb->write_index;
		}
	}
	else
	{
		rb->write_index++;
		if (old_state == RT_RINGBUFFER_FULL)
			rb->read_index = rb->write_index;
	}

	return 1;
}

/**
 * get a character from a ringbuffer
 */
uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch)
{
	if(NULL == rb)return 0;

	/* ringbuffer is empty */
	if (!rt_ringbuffer_data_len(rb))
		return 0;

	/* put character */
	*ch = rb->buffer_ptr[rb->read_index];

	if (rb->read_index == rb->buffer_size-1)
	{
		rb->read_mirror = ~rb->read_mirror;
		rb->read_index = 0;
	}
	else
	{
		rb->read_index++;
	}

	return 1;
}
/**
 * get the size of data in rb
 */
uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
	switch (rt_ringbuffer_status(rb))
	{
	case RT_RINGBUFFER_EMPTY:
		return 0;
	case RT_RINGBUFFER_FULL:
		return rb->buffer_size;
	case RT_RINGBUFFER_HALFFULL:
	default:
		if (rb->write_index > rb->read_index)
			return rb->write_index - rb->read_index;
		else
			return rb->buffer_size - (rb->read_index - rb->write_index);
	};
}

/**
 * empty the rb
 */
void rt_ringbuffer_reset(struct rt_ringbuffer *rb)
{
	if(NULL == rb)return;

	rb->read_mirror = 0;
	rb->read_index = 0;
	rb->write_mirror = 0;
	rb->write_index = 0;
}

struct rt_ringbuffer* rt_ringbuffer_create(uint16_t size)
{
	struct rt_ringbuffer *rb;
	uint8_t *pool;

	if(size <= 0)return 0;

	size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);

	rb = malloc(sizeof(struct rt_ringbuffer));
	if (rb == NULL)
		goto exit;

	pool = malloc(size);
	if (pool == NULL)
	{
		free(rb);
		rb = NULL;
		goto exit;
	}
	rt_ringbuffer_init(rb, pool, size);

	exit:
	return rb;
}

void rt_ringbuffer_destroy(struct rt_ringbuffer *rb)
{
	if(NULL == rb)return;

	free(rb->buffer_ptr);
	free(rb);
}


uint16_t rt_ringbuffer_get_size(struct rt_ringbuffer *rb)
{
	return rb->buffer_size;
}

void ringBufferTest(void)
{
	rb_example_t data;
	data.a = 1;
	data.b = 2;

	struct rt_ringbuffer * rb = rt_ringbuffer_create(sizeof(rb_example_t) * 2);
	if(NULL == rb)return;

	rt_ringbuffer_put(rb, (uint8_t *)&data, sizeof(data));


	rb_example_t recv_data;
	uint32_t recv = rt_ringbuffer_get(rb, (uint8_t *)&recv_data, sizeof(recv_data));
	if(recv != sizeof(recv_data))return;
}
/********************extern_function_end  *****************************************/
