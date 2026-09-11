//
//  rpl_stream.h
//  librpl
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stream__h__
#define __RPL__rpl_stream__h__

#include "rpl_defines.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>


RPL_HEADER_BEGIN


/*! An input/output stream. */
typedef struct rpl_stream *rpl_stream_t;


/*! Stream direction. */
typedef enum rpl_stream_direction {

    /*! If the direction of a stream is unknown, it's closed. */
    rpl_stream_direction_unknown = 0,

    /*! A stream that will be used only for input. */
    rpl_stream_direction_input  = 1,

    /*! A stream that will be used only for output. */
    rpl_stream_direction_output = 2,

    /*! A stream that will be used for both input and output. */
    rpl_stream_direction_inout  = 3,
} rpl_stream_direction_t;


/*! Seek whence. */
typedef enum rpl_stream_whence {
    rpl_stream_whence_set = 0,
    rpl_stream_whence_current = 1,
    rpl_stream_whence_end = 2,
} rpl_stream_whence_t;


/*!
 Callbacks for stream implementations.

 All functions should return `0` on success or `-errno` on error unless
 otherwise noted.
 */
typedef struct rpl_stream_callbacks {
    /*! Auxiliary data used by this instance of the stream callbacks. */
    void * RPL_NULLABLE aux;

    /* Mandatory functions. */

    int (*open)(rpl_stream_t stream, const char *path,
		rpl_stream_direction_t direction,
		void * RPL_NULLABLE options, va_list args);

    int (*close)(rpl_stream_t stream);

    /*!
     Read from a stream.

     @return the number of bytes read or `-errno` on error
     */
    ssize_t (*read)(rpl_stream_t stream,
		    void *buf, size_t buf_size);

    /*!
     Write to a stream.

     @return the number of bytes written or `-errno` on error
     */
    ssize_t (*write)(rpl_stream_t stream,
		     void *buf, size_t buf_size);

    /* Optional functions. */

    /*!
     Indicates whether the stream is at its end.

     @return `1` if the stream is at its end, `0` if it's not, or
	     `-errno` on error
     */
    int (* RPL_NULLABLE at_end)(rpl_stream_t stream);

    /*! Seek the stream to @a offset bytes relative to @a whence. */
    int (* RPL_NULLABLE seek)(rpl_stream_t stream, off_t offset,
			      rpl_stream_whence_t whence);

    /*!
     Return the current byte offset in the stream.

     @return the byte offset or `-errno`
     */
    off_t (* RPL_NULLABLE tell)(rpl_stream_t stream);

    /*! Free stream callbacks auxiliary data. */
    void (*free_aux)(rpl_stream_t stream, void * RPL_NULLABLE aux);
} rpl_stream_callbacks_t;


/*!
 Get callbacks enabling the use of a `FILE *` as a stream.

 The lifetime of any passed `FILE *` must be managed by the caller;
 while `rpl_stream_open` and `rpl_stream_close` must still be called to
 properly maintaain the stream's state, they **will not** invoke
 ``fopen(3)`` and ``fclose(3)`` if a `FILE *` is passed when creataing
 calbaacks.
 */
rpl_stream_callbacks_t * RPL_NULLABLE
rpl_stream_new_FILE_callbacks(FILE * RPL_NULLABLE file);


/*!
 Create a new stream using the given set of stream callbacks and refcon.
 
 The stream will take ownership of the callbacks and use ``free(3)`` to
 dispose of them when it is itself disposed of.
 */
rpl_stream_t RPL_NULLABLE
rpl_stream_new(rpl_stream_callbacks_t *callbacks,
	       void * RPL_NULLABLE refcon);

void
rpl_stream_free(rpl_stream_t stream);

void * RPL_NULLABLE
rpl_stream_get_refcon(rpl_stream_t stream);

int
rpl_stream_get_errno(rpl_stream_t stream);

bool
rpl_stream_open(rpl_stream_t stream, const char *path,
		rpl_stream_direction_t direction,
		void * RPL_NULLABLE options, va_list args);

bool
rpl_stream_close(rpl_stream_t stream);

ssize_t
rpl_stream_read(rpl_stream_t stream, void *buf, size_t buf_size);

ssize_t
rpl_stream_write(rpl_stream_t stream, void *buf, size_t buf_size);

int rpl_stream_at_end(rpl_stream_t stream);

bool rpl_stream_seek(rpl_stream_t stream, off_t offset,
		     rpl_stream_whence_t whence);

off_t rpl_stream_tell(rpl_stream_t stream);


RPL_HEADER_END


#endif /* __RPL__rpl_stream__h__ */
