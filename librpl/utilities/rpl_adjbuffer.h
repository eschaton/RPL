//
//  rpl_adjbuffer.h
//  RPL
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_adjbuffer__h__
#define __RPL__rpl_adjbuffer__h__

#include "rpl_defines.h"

#include <stdbool.h>
#include <unistd.h>


RPL_HEADER_BEGIN


/*!
 An adjustable buffer is an array whose elements may be structures of
 arbitrary (but uniform) size, and into which items may be inserted and
 removed.
 */
struct rpl_adjbuffer {
    void *_storage;
    size_t _count;
    size_t _capacity;
    size_t _element_size;
};
typedef struct rpl_adjbuffer rpl_adjbuffer_t;


/*! Allocate a new buffer whose elements are @a element_size bytes. */
rpl_adjbuffer_t * RPL_NULLABLE
rpl_adjbuffer_new(size_t capacity, size_t element_size);

/*! Initialize a new buffer whose elements are @a element_size bytes. */
bool
rpl_adjbuffer_init(rpl_adjbuffer_t *buffer, size_t capacity,
		   size_t element_size);

/*! Dispose of a buffer's internal data structures. */
void
rpl_adjbuffer_deinit(rpl_adjbuffer_t *buffer);

/*! Dispose of a buffer. */
void
rpl_adjbuffer_free(rpl_adjbuffer_t *buffer);

/*! Return the number of elements in a buffer. */
size_t
rpl_adjbuffer_get_count(rpl_adjbuffer_t *buffer);

/*! Set the @a element of @a buffer at index @a idx. */
void
rpl_adjbuffer_set(rpl_adjbuffer_t *buffer, size_t idx, void *element);

/*! Get a pointer to the element of @a buffer at index @a idx. */
void *
rpl_adjbuffer_get(rpl_adjbuffer_t *buffer, size_t idx);

/*! Insert @a element into @a buffer at index @a idx. */
bool
rpl_adjbuffer_insert_element(rpl_adjbuffer_t *buffer, size_t idx,
			     void *element);

/*! Insert @a elements into @a buffer starting at index @a idx. */
bool
rpl_adjbuffer_insert_elements(rpl_adjbuffer_t *buffer, size_t idx,
			      const void *elements,
			      size_t elements_count);

/*! Remove the element at index @a idx from @a buffer. */
void
rpl_adjbuffer_remove_element(rpl_adjbuffer_t *buffer, size_t idx);

/*!
 Remove @a elements_count elements starting at @a idx from @a buffer.
 */
void
rpl_adjbuffer_remove_elements(rpl_adjbuffer_t *buffer, size_t idx,
			      size_t elements_count);

/*!
 Remove all elements from @a buffer.
 */
void
rpl_adjbuffer_remove_all_elements(rpl_adjbuffer_t *buffer);

/*! Append @a element to the end of @a buffer. */
bool
rpl_adjbuffer_append_element(rpl_adjbuffer_t *buffer,
			     const void *element);

/*! Append @a elements to the end of @a buffer. */
bool
rpl_adjbuffer_append_elements(rpl_adjbuffer_t *buffer,
			      const void *elements,
			      size_t elements_count);

/*! A function that can be applied to a buffer's elements. */
typedef bool (*rpl_adjbuffer_apply_f)(rpl_adjbuffer_t *buffer,
				      void *element, size_t idx,
				      void * RPL_NULLABLE refcon);

/*!
 Apply a function to a buffer's elements.

 The function may modify the elements but not the array itself.

 @returns `true` on complete iteration, `false` otherwise
 */
bool
rpl_adjbuffer_apply(rpl_adjbuffer_t *buffer,
		    rpl_adjbuffer_apply_f function,
		    void * RPL_NULLABLE refcon);



RPL_HEADER_END


#endif /* __RPL__rpl_adjbuffer__h__ */
