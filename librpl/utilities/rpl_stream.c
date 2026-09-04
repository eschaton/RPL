//
//  rpl_stream.c
//  RPL
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_stream_internal.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


rpl_stream_t RPL_NULLABLE
rpl_stream_new(rpl_stream_callbacks_t *callbacks,
	       void * RPL_NULLABLE refcon)
{
    assert(callbacks != NULL);
    
    rpl_stream_t stream = calloc(1, sizeof(struct rpl_stream));
    if (stream) {
	stream->_callbacks = callbacks;
	stream->_direction = rpl_stream_direction_unknown;
	stream->_refcon = refcon;
    }
    return stream;
}

void
rpl_stream_free(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction == rpl_stream_direction_unknown);
    
    if (stream->_callbacks->free_aux) {
	stream->_callbacks->free_aux(stream, stream->_callbacks->aux);
    }
    
    free(stream->_callbacks);
    free(stream);
}

void * RPL_NULLABLE
rpl_stream_get_refcon(rpl_stream_t stream)
{
    assert(stream != NULL);

    return stream->_refcon;
}

int
rpl_stream_get_errno(rpl_stream_t stream)
{
    assert(stream != NULL);

    return stream->_errno;
}

bool
rpl_stream_open(rpl_stream_t stream, const char *path,
		rpl_stream_direction_t direction,
		void * RPL_NULLABLE options, va_list args)
{
    assert(stream != NULL);
    assert(stream->_direction == rpl_stream_direction_unknown);
    assert(path != NULL);

    int err = stream->_callbacks->open(stream, path, direction, options,
				       args);
    if (err == 0) stream->_errno = 0;
    else stream->_errno = -err;

    return (err == 0);
}

bool
rpl_stream_close(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);

    int err = stream->_callbacks->close(stream);
    if (err == 0) stream->_errno = 0;
    else stream->_errno = -err;

    /* Mark stream as closed regardless of error. */
    
    stream->_direction = rpl_stream_direction_unknown;

    return (err == 0);
}

ssize_t
rpl_stream_read(rpl_stream_t stream, void *buf, size_t buf_size)
{
    assert(stream != NULL);
    assert((stream->_direction == rpl_stream_direction_input)
	   || (stream->_direction == rpl_stream_direction_inout));
    assert(buf_size < (SIZE_MAX / 2));

    ssize_t result = stream->_callbacks->read(stream, buf, buf_size);
    if (result >= 0) stream->_errno = 0;
    else stream->_errno = (int) -result;

    return (result >= 0) ? result : -1;
}

ssize_t
rpl_stream_write(rpl_stream_t stream, void *buf, size_t buf_size)
{
    assert(stream != NULL);
    assert((stream->_direction == rpl_stream_direction_output)
	   || (stream->_direction == rpl_stream_direction_inout));
    assert(buf_size < (SIZE_MAX / 2));

    ssize_t result = stream->_callbacks->write(stream, buf, buf_size);
    if (result >= 0) stream->_errno = 0;
    else stream->_errno = (int) -result;

    return (result >= 0) ? result : -1;
}

int rpl_stream_at_end(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    assert(stream->_callbacks->at_end != NULL);

    int result = stream->_callbacks->at_end(stream);
    if (result >= 0) stream->_errno = 0;
    else stream->_errno = -result;

    return result;
}

bool rpl_stream_seek(rpl_stream_t stream, off_t offset,
		     rpl_stream_whence_t whence)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    assert(stream->_callbacks->at_end != NULL);

    int err = stream->_callbacks->seek(stream, offset, whence);
    if (err == 0) stream->_errno = 0;
    else stream->_errno = (int) -err;

    return (err == 0);
}

off_t rpl_stream_tell(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    assert(stream->_callbacks->at_end != NULL);

    off_t result = stream->_callbacks->tell(stream);
    if (result >= 0) stream->_errno = 0;
    else stream->_errno = (int) -result;

    return (result >= 0) ? result : -1;
}


/* MARK: - FILE streams */

rpl_stream_callbacks_t rpl_stream_callbacks_FILE = {
    NULL,

    rpl_stream_FILE_open,
    rpl_stream_FILE_close,
    rpl_stream_FILE_read,
    rpl_stream_FILE_write,

    rpl_stream_FILE_at_end,
    rpl_stream_FILE_seek,
    rpl_stream_FILE_tell,
    
    rpl_stream_FILE_free_aux,
};

typedef struct rpl_stream_FILE_aux {
    FILE *_file;
    const char *_path;
} rpl_stream_FILE_aux_t;

rpl_stream_callbacks_t * RPL_NULLABLE
rpl_stream_get_FILE_callbacks(FILE * RPL_NULLABLE file)
{
    rpl_stream_callbacks_t *callbacks = NULL;
    rpl_stream_FILE_aux_t *aux = NULL;

    callbacks = calloc(1, sizeof(rpl_stream_callbacks_t));
    if (callbacks == NULL) goto error;

    aux = calloc(1, sizeof(rpl_stream_FILE_aux_t));
    if (aux == NULL) goto error;

    memcpy(callbacks, &rpl_stream_callbacks_FILE,
	   sizeof(rpl_stream_callbacks_t));

    callbacks->aux = aux;
    aux->_file = file;

error:
    if (callbacks) free(callbacks);
    if (aux) free(aux);
    return NULL;
}

rpl_stream_FILE_aux_t *
rpl_stream_FILE_get_aux(rpl_stream_t stream)
{
    assert(stream != NULL);

    return stream->_callbacks->aux;
}

const char * RPL_NULLABLE
rpl_stream_FILE_mode_for_direction(rpl_stream_direction_t direction)
{
    switch (direction) {
	case rpl_stream_direction_unknown: return NULL;
	case rpl_stream_direction_input:   return "r";
	case rpl_stream_direction_output:  return "w";
	case rpl_stream_direction_inout:   return "rw";
    }
}

int
rpl_stream_FILE_open(rpl_stream_t stream, const char *path,
		     rpl_stream_direction_t direction,
		     void * RPL_NULLABLE options, va_list args)
{
    assert(stream != NULL);
    assert(path != NULL);
    assert(direction != rpl_stream_direction_unknown);
    assert(stream->_direction == rpl_stream_direction_unknown);

    int saved_errno = 0;
    FILE *file = NULL;
    const char *pathdup = NULL;

    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);

    if (aux->_file == NULL) {
	/* Options & args are ignored for FILE streams. */
	const char *mode
	    = rpl_stream_FILE_mode_for_direction(direction);
	assert(mode != NULL);

	file = fopen(path, mode);

	if (file == NULL) {
	    saved_errno = errno;
	    goto error;
	}

	pathdup = strdup(path);
	if (pathdup == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	}

	stream->_direction = direction;
	aux->_file = file;
	aux->_path = pathdup;
    } else {
	/*
	 The file is already set, so just record our information and
	 hope it matches (since there's no way to recover it from a C
	 stdio FILE *).
	 */

	stream->_direction = direction;

	file = aux->_file;

	pathdup = strdup(path);
	if (pathdup == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	}
    }

    return 0;

error:
    if (file) fclose(file);
    return -saved_errno;
}

int
rpl_stream_FILE_close(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    
    int saved_errno = 0;
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    int err = fclose(aux->_file);
    if (err != 0) {
	saved_errno = errno;
	goto error;
    }
    
    return 0;
    
error:
    return -saved_errno;
}

ssize_t
rpl_stream_FILE_read(rpl_stream_t stream, void *buf, size_t buf_size)
{
    assert(stream != NULL);
    assert((stream->_direction == rpl_stream_direction_input)
	   || (stream->_direction == rpl_stream_direction_inout));
    assert(buf_size < (SIZE_MAX / 2));

    int saved_errno = 0;
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    size_t result = fread(buf, buf_size, 1, aux->_file);
    if (result != 1) {
	saved_errno = errno;
	if (feof(aux->_file)) {
	    /* In case of EOF, return 0. */
	    saved_errno = 0;
	}
	goto error;
    }
    
    return buf_size;
    
error:
    return -saved_errno;
}

ssize_t
rpl_stream_FILE_write(rpl_stream_t stream, void *buf, size_t buf_size)
{
    assert(stream != NULL);
    assert((stream->_direction == rpl_stream_direction_output)
	   || (stream->_direction == rpl_stream_direction_inout));
    assert(buf_size < (SIZE_MAX / 2));
    
    int saved_errno = 0;
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    size_t result = fwrite(buf, buf_size, 1, aux->_file);
    if (result != 1) {
	saved_errno = errno;
	goto error;
    }
    
    return buf_size;
    
error:
    return -saved_errno;
}

int rpl_stream_FILE_at_end(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    /* C claims feof(3) cannot fail. */
    
    return feof(aux->_file) != 0 ? 1 : 0;
}

int rpl_stream_FILE_seek(rpl_stream_t stream, off_t offset,
			 rpl_stream_whence_t whence)
{
    assert(stream != NULL);
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    
    int saved_errno = 0;
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    int fwhence;
    switch (whence) {
	case rpl_stream_whence_set:     fwhence = SEEK_SET; break;
	case rpl_stream_whence_current: fwhence = SEEK_CUR; break;
	case rpl_stream_whence_end:     fwhence = SEEK_END; break;
    }
    
    int err = fseeko(aux->_file, offset, fwhence);
    if (err == -1) {
	saved_errno = -1;
	goto error;
    }
    
    return 0;
    
error:
    return -saved_errno;
}

off_t rpl_stream_FILE_tell(rpl_stream_t stream)
{
    assert(stream != NULL);
    assert(stream->_direction != rpl_stream_direction_unknown);
    
    off_t offset;
    int saved_errno = 0;
    
    rpl_stream_FILE_aux_t *aux = rpl_stream_FILE_get_aux(stream);
    assert(aux != NULL);
    
    offset = ftello(aux->_file);
    if (offset == -1) {
	saved_errno = errno;
	goto error;
    }
    
    return offset;
    
error:
    return -saved_errno;
}

void rpl_stream_FILE_free_aux(rpl_stream_t stream,
			      void * RPL_NULLABLE aux)
{
    assert(stream != NULL);
    assert(stream->_direction == rpl_stream_direction_unknown);
    assert(aux != NULL);
    
    free(aux);
}


RPL_SOURCE_END
