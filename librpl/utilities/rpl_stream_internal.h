//
//  rpl_stream_internal.h
//  RPL
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stream_internal__h__
#define __RPL__rpl_stream_internal__h__

#include "rpl_stream.h"


RPL_HEADER_BEGIN


struct rpl_stream {
    rpl_stream_callbacks_t *_callbacks;
    rpl_stream_direction_t _direction;
    void * RPL_NULLABLE _refcon;
    int _errno;
};


/* MARK: - FILE streams */

int rpl_stream_FILE_open(rpl_stream_t stream, const char *path,
			 rpl_stream_direction_t direction,
			 void * RPL_NULLABLE options, va_list args);

int rpl_stream_FILE_close(rpl_stream_t stream);

ssize_t rpl_stream_FILE_read(rpl_stream_t stream,
			     void *buf, size_t buf_size);

ssize_t rpl_stream_FILE_write(rpl_stream_t stream,
			      void *buf, size_t buf_size);

int rpl_stream_FILE_at_end(rpl_stream_t stream);

int rpl_stream_FILE_seek(rpl_stream_t stream, off_t offset,
			 rpl_stream_whence_t whence);

off_t rpl_stream_FILE_tell(rpl_stream_t stream);

void rpl_stream_FILE_free_aux(rpl_stream_t stream,
			      void * RPL_NULLABLE aux);


RPL_HEADER_END


#endif /* __RPL__rpl_stream_internal__h__ */
