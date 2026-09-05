//
//  rpl_tokenizer.c
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_tokenizer_internal.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


/*
 RPL syntax is straightforward:

 integer := '#' /[0-9A-Fa-f]+[bdoh]?/.
 real := /[+-]?[0-9]*(.[0-9]*)?([eE][+-][0-9]+)?/.
 complex := '(' real ',' /[∡∢]?/ real ')'.
 array_element := real | complex | array.
 array := '[' array_element* ']'.
 name_text := character+.
 name := '\'' name_text '\''.
 program := '«' token+ '»'.
 string := '"' character* '"'.
 list := '{' value* '}'.
 tagged := ':' name_text ':' value.
 unit := real '_' name_text.
 value := integer | real | complex | array | name | program
	| string | list | tagged | unit.
 identifier := name_text.
 comment := '@' character* end_of_line.

 Almost all syntactic units in the grammar above are separable by
 whitespace other than the following:

 - @ by itself starts a comment that spans to the next end of line
 - name_text does not allow whitespace, ', or "
 - string requires backslash-quoting before " and \ and supports the
   usual quoted C characters (\\, \t, \r, \n)
 - tagged does not allow whitespace between syntactic subunits
 - unit does not allow whitespace between syntactic subunits
 - array, program, and list support nesting

 Identifiers are used to represent operations and are immediately
 evaluated.
 */


rpl_tokenizer_t RPL_NULLABLE
rpl_tokenizer_new(rpl_context_t context)
{
    assert(context != NULL);

    rpl_tokenizer_t tokenizer = calloc(1, sizeof(struct rpl_tokenizer));
    if (tokenizer) {
	tokenizer->_context = context;
	tokenizer->_strbuffer = rpl_strbuffer_new_empty(256);
	if (tokenizer->_strbuffer == NULL) goto error;
	tokenizer->_cur = -1;
    }

    return tokenizer;

error:
    rpl_tokenizer_free(tokenizer);
    return NULL;
}

RPL_EXPORT
void
rpl_tokenizer_free(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    if (tokenizer->_strbuffer) {
	rpl_strbuffer_free(tokenizer->_strbuffer);
    }

    free(tokenizer);
}

bool
rpl_tokenizer_append(rpl_tokenizer_t tokenizer, const char *str)
{
    assert(str != NULL);
    assert(strlen(str) > 0);

    rpl_strbuffer_t sb = tokenizer->_strbuffer;

    if (tokenizer->_cur == rpl_strbuffer_get_length(sb)) {
	/*
	 If the buffer has been entirely consumed, reset it and the
	 tokenizer.
	 */

	rpl_strbuffer_remove_all(sb);
	tokenizer->_cur = 0;
    }

    return rpl_strbuffer_append_chars(tokenizer->_strbuffer, str);
}

char
rpl_tokenizer_get_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    char ch;
    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_strbuffer_t sb = tokenizer->_strbuffer;
	ch = rpl_strbuffer_get_char(sb, tokenizer->_cur);
	tokenizer->_cur += 1;
    } else {
	ch = '\0';
    }

    return ch;
}

void
rpl_tokenizer_unget_char(rpl_tokenizer_t tokenizer, char ch)
{
    assert(tokenizer != NULL);
    assert(tokenizer->_cur > 0);
    assert(ch != '\0');

    rpl_strbuffer_t sb = tokenizer->_strbuffer;
    tokenizer->_cur -= 1;

    assert(rpl_strbuffer_get_char(sb, tokenizer->_cur) == ch);
}

char
rpl_tokenizer_peek_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    char ch = '\0';

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_strbuffer_t sb = tokenizer->_strbuffer;
	ch = rpl_strbuffer_get_char(sb, tokenizer->_cur);
    }

    return ch;
}

bool
rpl_tokenizer_has_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    return (tokenizer->_cur != -1);
}

bool
rpl_char_is_whitespace(char ch)
{
    return ((ch == ' ') || (ch == '\t')
	    || (ch == '\n') || (ch == '\r'));
}

bool
rpl_char_is_comment_start(char ch)
{
    return (ch == '@');
}

bool
rpl_char_is_integer_start(char ch)
{
    return ch == '#';
}

bool
rpl_char_is_digit(char ch)
{
    return isdigit(ch);
}

bool
rpl_char_is_real_start(char ch)
{
    // TODO: rpl_char_is_real_start
    return false;
}

bool
rpl_char_is_complex_start(char ch)
{
    return ch == '(';
}

bool
rpl_char_is_array_start(char ch)
{
    return ch == '[';
}

bool
rpl_char_is_name_start(char ch)
{
    return ch == '\'';
}

bool
rpl_char_is_program_start(char ch)
{
    // TODO: rpl_char_is_program_start
    return false;
}

bool
rpl_char_is_string_start(char ch)
{
    return ch == '"';
}

bool
rpl_char_is_list_start(char ch)
{
    return ch == '{';
}

bool
rpl_char_is_tagged_start(char ch)
{
    return ch == ':';
}

bool
rpl_char_is_identifier_start(char ch)
{
    /*
     Anything that isn't whitespace and doesn't start another token
     (except a real, since there's overlap) can start an identifier.
     */

    return (!rpl_char_is_whitespace(ch)
	    && !rpl_char_is_comment_start(ch)
	    && !rpl_char_is_integer_start(ch)
	    && !rpl_char_is_digit(ch)
	    && !rpl_char_is_complex_start(ch)
	    && !rpl_char_is_array_start(ch)
	    && !rpl_char_is_name_start(ch)
	    && !rpl_char_is_program_start(ch)
	    && !rpl_char_is_string_start(ch)
	    && !rpl_char_is_list_start(ch)
	    && !rpl_char_is_tagged_start(ch));
}

void
rpl_tokenizer_skip_whitespace(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    while (rpl_tokenizer_has_char(tokenizer)) {
	char ch = rpl_tokenizer_get_char(tokenizer);
	if (rpl_char_is_whitespace(ch) == false) {
	    rpl_tokenizer_unget_char(tokenizer, ch);
	    break;
	}
    }
}

void
rpl_tokenizer_skip_comment(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    ssize_t saved = tokenizer->_cur;

    char ch = rpl_tokenizer_get_char(tokenizer);
    assert(rpl_char_is_comment_start(ch));

    bool saw_eol = false;
    while (rpl_tokenizer_has_char(tokenizer)) {
	ch = rpl_tokenizer_get_char(tokenizer);
	if ((ch == '\r') || (ch == '\n')) {
	    saw_eol = true;
	    break;
	}
    }

    /* We don't have a complete line, rewind. */

    if (saw_eol == false) {
	tokenizer->_cur = saved;
    }
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_get_next(rpl_tokenizer_t tokenizer)
{
    rpl_token_t token = NULL;

    /* Skip whitespace and comments. */

    if (rpl_tokenizer_has_char(tokenizer)) {
	char ch = rpl_tokenizer_peek_char(tokenizer);

	if (rpl_char_is_whitespace(ch)) {
	    rpl_tokenizer_skip_whitespace(tokenizer);
	} else if (rpl_char_is_comment_start(ch)) {
	    rpl_tokenizer_skip_comment(tokenizer);
	}
    }

    /* Figure out what comes next. */

    if (rpl_tokenizer_has_char(tokenizer)) {
	char ch = rpl_tokenizer_peek_char(tokenizer);

	if (rpl_char_is_integer_start(ch)) {
	    token = rpl_tokenizer_tokenize_integer(tokenizer);
	} else if (rpl_char_is_real_start(ch)) {
	    token = rpl_tokenizer_tokenize_real_or_unit(tokenizer);
	} else if (rpl_char_is_complex_start(ch)) {
	    token = rpl_tokenizer_tokenize_complex(tokenizer);
	} else if (rpl_char_is_array_start(ch)) {
	    token = rpl_tokenizer_tokenize_array(tokenizer);
	} else if (rpl_char_is_name_start(ch)) {
	    token = rpl_tokenizer_tokenize_name(tokenizer);
	} else if (rpl_char_is_program_start(ch)) {
	    token = rpl_tokenizer_tokenize_program(tokenizer);
	} else if (rpl_char_is_string_start(ch)) {
	    token = rpl_tokenizer_tokenize_string(tokenizer);
	} else if (rpl_char_is_list_start(ch)) {
	    token = rpl_tokenizer_tokenize_list(tokenizer);
	} else if (rpl_char_is_tagged_start(ch)) {
	    token = rpl_tokenizer_tokenize_tagged(tokenizer);
	} else {
	    token = rpl_tokenizer_tokenize_identifier(tokenizer);
	}
    }

    return token;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_integer(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_integer
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_real_or_unit(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_real_or_unit
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_complex(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_complex
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_array(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_array
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_name(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_name
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_program(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_program
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_string(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_string
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_list(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_list
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_tagged(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_tagged
    return NULL;
}

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_identifier(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_identifier
    return NULL;
}


RPL_SOURCE_END
