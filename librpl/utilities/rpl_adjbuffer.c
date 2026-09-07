//
//  rpl_adjbuffer.c
//  RPL
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_adjbuffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


/*! Amount by which capacity increases, also used for base capacity. */
const size_t rpl_adjbuffer_quantum = 8;


/*! Round to the next quantum. */
size_t
rpl_adjbuffer_round_to_next(size_t value)
{
    return value + (rpl_adjbuffer_quantum
		    - (value % rpl_adjbuffer_quantum));
}

/*!
 Adjust the storage of a buffer if it's about to be filled.

 @returns `false` if allocation failed, `true` otherwise
 */
bool
rpl_adjbuffer_adjust_storage(rpl_adjbuffer_t *buffer, size_t count)
{
    assert(buffer != NULL);

    if (buffer->_count + count >= buffer->_capacity) {
	const size_t esize = buffer->_element_size;
	const size_t ocap = buffer->_capacity;
	const size_t ncap = rpl_adjbuffer_round_to_next(ocap + count);
	const size_t osize = ocap * esize;
	const size_t nsize = ncap * esize;
	void *ostorage = buffer->_storage;
	void *nstorage = realloc(ostorage, nsize);
	if (nstorage == NULL) goto error;
	void *nextra = nstorage + osize;
	memset(nextra, 0, nsize - osize);
	buffer->_storage = nstorage;
	buffer->_capacity = ncap;
    }

    return true;

error:
    return false;
}

rpl_adjbuffer_t * RPL_NULLABLE
rpl_adjbuffer_new(size_t capacity, size_t element_size)
{
    assert(element_size > 0);

    rpl_adjbuffer_t *buffer = calloc(1, sizeof(rpl_adjbuffer_t));
    if (buffer) {
	bool initialized
	    = rpl_adjbuffer_init(buffer, capacity, element_size);
	if (!initialized) goto error;
    }
    return buffer;

error:
    rpl_adjbuffer_free(buffer);
    return NULL;
}

bool
rpl_adjbuffer_init(rpl_adjbuffer_t *buffer, size_t capacity,
		   size_t element_size)
{
    assert(buffer != NULL);
    assert(element_size > 0);

    const size_t real_capacity = rpl_adjbuffer_round_to_next(capacity);
    buffer->_storage = calloc(real_capacity, element_size);
    if (buffer->_storage == NULL) goto error;
    buffer->_count = 0;
    buffer->_capacity = real_capacity;
    buffer->_element_size = element_size;

    return true;

error:
    return false;
}

void
rpl_adjbuffer_deinit(rpl_adjbuffer_t *buffer)
{
    assert(buffer != NULL);

    free(buffer->_storage);
}

void
rpl_adjbuffer_free(rpl_adjbuffer_t *buffer)
{
    assert(buffer != NULL);

    rpl_adjbuffer_deinit(buffer);

    free(buffer);
}

size_t
rpl_adjbuffer_get_count(rpl_adjbuffer_t *buffer)
{
    assert(buffer != NULL);

    return buffer->_count;
}

void *
rpl_adjbuffer_get(rpl_adjbuffer_t *buffer, size_t idx)
{
    assert(buffer != NULL);
    assert(idx < buffer->_count);

    void *location = buffer->_storage + (buffer->_element_size * idx);

    return location;
}

void
rpl_adjbuffer_set(rpl_adjbuffer_t *buffer, size_t idx, void *element)
{
    assert(buffer != NULL);
    assert(idx < buffer->_count);
    assert(element != NULL);

    void *location = buffer->_storage + (buffer->_element_size * idx);

    memcpy(location, element, buffer->_element_size);
}

bool
rpl_adjbuffer_insert_element(rpl_adjbuffer_t *buffer, size_t idx,
			     void *element)
{
    assert(buffer != NULL);
    assert(idx <= buffer->_count);
    assert(element != NULL);

    return rpl_adjbuffer_insert_elements(buffer, idx, element, 1);
}

bool
rpl_adjbuffer_insert_elements(rpl_adjbuffer_t *buffer, size_t idx,
			      const void *elements,
			      size_t elements_count)
{
    assert(buffer != NULL);
    assert(idx <= buffer->_count);
    assert(elements != NULL);
    assert(elements_count > 0);

    bool adjusted = rpl_adjbuffer_adjust_storage(buffer,
						 elements_count);
    if (!adjusted) goto error;

    const size_t ocount = buffer->_count;
    const size_t esize = buffer->_element_size;
    void *storage = buffer->_storage;

    void *eidx = storage + (idx * esize);
    void *eidx_next = storage + ((idx + elements_count) * esize);
    const size_t movecount = ocount - idx;
    if (movecount > 0) {
	/*
	 Move all elements from idx through ocount to idx +
	 elements_count, leaving a hole for the inserted elements.
	 */

	const size_t movelen = movecount * esize;
	memmove(eidx_next, eidx, movelen);
    }

    /* Copy in elements starting at idx. */
    memcpy(eidx, elements, esize * elements_count);

    buffer->_count += elements_count;

    return true;

error:
    return false;
}

void
rpl_adjbuffer_remove_element(rpl_adjbuffer_t *buffer, size_t idx)
{
    assert(buffer != NULL);
    assert(idx < buffer->_count);

    rpl_adjbuffer_remove_elements(buffer, idx, 1);
}

void
rpl_adjbuffer_remove_elements(rpl_adjbuffer_t *buffer, size_t idx,
			      size_t elements_count)
{
    assert(buffer != NULL);
    assert(idx < buffer->_count);
    assert(elements_count > 0);

    const size_t ocount = buffer->_count;
    const size_t esize = buffer->_element_size;
    void *storage = buffer->_storage;

    void *eidx = storage + (idx * esize);
    void *eidx_next = storage + ((idx + elements_count) * esize);
    void *elast = storage + (ocount * esize);
    const size_t movecount = ocount - idx - 1;
    if (movecount > 0) {
	/*
	 Move all elements from idx+elements_count through ocount to
	 idx, closing the hole from the removed elements.
	 */

	const size_t movelen = movecount * esize;
	memmove(eidx, eidx_next, movelen);
    }

    /* Clear element(s) at end. */
    memset(elast, 0, esize * elements_count);

    buffer->_count -= 1;
}

void
rpl_adjbuffer_remove_all_elements(rpl_adjbuffer_t *buffer)
{
    assert(buffer != NULL);

    memset(buffer->_storage, 0,
	   buffer->_capacity * buffer->_element_size);
    
    buffer->_count = 0;
}

bool
rpl_adjbuffer_append_element(rpl_adjbuffer_t *buffer,
			     const void *element)
{
    assert(buffer != NULL);
    assert(element != NULL);

    return rpl_adjbuffer_append_elements(buffer, element, 1);
}

bool
rpl_adjbuffer_append_elements(rpl_adjbuffer_t *buffer,
			      const void *elements,
			      size_t elements_count)
{
    assert(buffer != NULL);
    assert(elements != NULL);
    assert(elements_count > 0);

    return rpl_adjbuffer_insert_elements(buffer, buffer->_count,
					 elements, elements_count);
}

bool
rpl_adjbuffer_apply(rpl_adjbuffer_t *buffer,
		    rpl_adjbuffer_apply_f function,
		    void * RPL_NULLABLE refcon)
{
    assert(buffer != NULL);
    assert(function != NULL);

    const size_t count = buffer->_count;
    bool stop = false;
    for (size_t i = 0; (i < count) && (stop == false); i++) {
	void *element = rpl_adjbuffer_get(buffer, i);
	stop = (*function)(buffer, element, i, refcon);
    }

    return (stop == false);
}


RPL_SOURCE_END
