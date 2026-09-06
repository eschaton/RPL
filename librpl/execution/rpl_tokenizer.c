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

#include "rpl_token_internal.h"


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

ssize_t
rpl_tokenizer_get_mark(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    return tokenizer->_cur;
}

void
rpl_tokenizer_set_mark(rpl_tokenizer_t tokenizer, ssize_t mark)
{
    assert(tokenizer != NULL);

    tokenizer->_cur = mark;
}

bool
rpl_char_is_whitespace(char ch)
{
    return ((ch == ' ') || (ch == '\t')
	    || (ch == '\n') || (ch == '\r'));
}

bool
rpl_char_is_delimiter(char ch)
{
    return (ch == '(') || (ch == ')')
	|| (ch == '[') || (ch == ']')
	|| (ch == '{') || (ch == '}')
	|| (ch == '"') || (ch == '\'');
}

bool
rpl_chars_are_unicode_delimiter(char chs[3])
{
    /* Only check for «» since those are the only syntactic ones. */

    return (chs[0] == (char)0xC2)
	&& ((chs[1] == (char)0xAB) || (chs[1] == (char)0xBB));
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
rpl_char_is_hexdigit(char ch)
{
    return rpl_char_is_digit(ch)
	|| ((ch >= 'A') && (ch <= 'F'))
	|| ((ch >= 'a') && (ch <= 'f'));
}

int
rpl_char_digit_value(char ch)
{
    if ((ch >= '0') && (ch <= '9')) {
	return ch - '0';
    } else if ((ch >= 'A') && (ch <= 'F')) {
	return ch - 'A';
    } else if ((ch >= 'a') && (ch <= 'f')) {
	return ch - 'a';
    } else {
	return -1;
    }
}

bool
rpl_char_is_base_indicator(char ch)
{
    return (ch == 'b') || (ch == 'd') || (ch == 'h') || (ch == 'o');
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
    return ch == (char)0xC2; /* C2 AB is start, C2 AB is end */
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

     Program start is special since it's just the first code point of a
     UTF-8 representation, that has to be checked separately when the
     identifier is tokenized.
     */

    return (!rpl_char_is_whitespace(ch)
	    && !rpl_char_is_comment_start(ch)
	    && !rpl_char_is_integer_start(ch)
	    && !rpl_char_is_digit(ch)
	    && !rpl_char_is_complex_start(ch)
	    && !rpl_char_is_array_start(ch)
	    && !rpl_char_is_name_start(ch)
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

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

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
	rpl_tokenizer_set_mark(tokenizer, mark);
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

	if ((token == NULL) && rpl_char_is_integer_start(ch)) {
	    token = rpl_tokenizer_tokenize_integer(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_real_start(ch)) {
	    token = rpl_tokenizer_tokenize_real_or_unit(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_complex_start(ch)) {
	    token = rpl_tokenizer_tokenize_complex(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_array_start(ch)) {
	    token = rpl_tokenizer_tokenize_array(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_name_start(ch)) {
	    token = rpl_tokenizer_tokenize_name(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_program_start(ch)) {
	    token = rpl_tokenizer_tokenize_program(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_string_start(ch)) {
	    token = rpl_tokenizer_tokenize_string(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_list_start(ch)) {
	    token = rpl_tokenizer_tokenize_list(tokenizer);
	}

	if ((token == NULL) && rpl_char_is_tagged_start(ch)) {
	    token = rpl_tokenizer_tokenize_tagged(tokenizer);
	}

	if (token == NULL) {
	    token = rpl_tokenizer_tokenize_identifier(tokenizer);
	}
    }

    return token;
}

/*!
 Tokenize a binary integer.

 Binary integers use the syntax

     integer := '#' ' '? /[0-9A-Fa-f]+[bdoh]?/.

 where _one space_ is allowed (but not required) between the `'#'` and
 the digits, and the allowed digits depend on either the current
 environment's integer base or the optional suffix.
 */
RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_integer(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_strbuffer_t buf = NULL;
    bool appended = false;

    enum parser_state {
	parser_state_start = 0,
	parser_state_saw_octothorpe,
	parser_state_accumulating_digits,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_strbuffer_new_empty(8);
    if (buf == NULL) goto back_out;

    int max_digit = -1;
    char base_indicator = '\0';

    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    char ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    assert(ch == '#');
		    state = parser_state_saw_octothorpe;
		} break;

		case parser_state_saw_octothorpe: {
		    if (ch != ' ') {
			/* Skip exactly one space. */
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		    state = parser_state_accumulating_digits;
		} break;

		case parser_state_accumulating_digits: {
		    if (rpl_char_is_hexdigit(ch)) {
			char chs[2] = { ch, '\0' };
			appended = rpl_strbuffer_append_chars(buf, chs);
			if (appended == false) goto back_out;

			int val = rpl_char_digit_value(ch);
			assert(val != -1);

			if (val > max_digit) max_digit = val;
		    } else {
			/*
			 A non-digit value ends the parse; unget it
			 unless it's one of the base indicators.
			 */
			if (rpl_char_is_base_indicator(ch)) {
			    base_indicator = ch;
			} else {
			    rpl_tokenizer_unget_char(tokenizer, ch);
			}
			state = parser_state_end;
		    }
		} break;

		case parser_state_end: {
		    /*
		     Shouldn't actually get here, the loop should exit.
		     */
		} break;
	    }
	}
    } while (state != parser_state_end);

    /* Some digits must have been read. */
    if (max_digit == -1) goto back_out;

    /*
     Ensure the digits agree with any base indicator.
     */
    if (base_indicator == 0) {
	/*
	 If no base indicator was part of the token, get the base from
	 the tokenizer's context's current environment.
	 */
	rpl_environment_t env
	    = rpl_context_get_environment(tokenizer->_context);
	const rpl_base_t base = rpl_environment_get_base(env);
	switch (base) {
	    case rpl_base_decimal:     base_indicator = 'd'; break;
	    case rpl_base_binary:      base_indicator = 'b'; break;
	    case rpl_base_octal:       base_indicator = 'o'; break;
	    case rpl_base_hexadecimal: base_indicator = 'h'; break;
	}
    }

    int base = 0;
    switch (base_indicator) {
	case 'b':
	    if (max_digit > 1) goto back_out;
	    base = 2;
	    break;
	case 'd':
	    if (max_digit > 9) goto back_out;
	    base = 10;
	    break;
	case 'o':
	    if (max_digit > 7) goto back_out;
	    base = 8;
	    break;
	case 'h':
	    if (max_digit > 15) goto back_out;
	    base = 16;
	    break;
    }

    /* Now that there are digits and a base, convert them to a value. */

    rpl_integer_t rep
	= strtoull(rpl_strbuffer_get_chars(buf), NULL, base);

    rpl_value_t val = rpl_integer_new(rep);
    if (val == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, val);
    if (token == NULL) goto back_out;

    rpl_strbuffer_free(buf);

back_out:
    if (buf) rpl_strbuffer_free(buf);
    if (token) rpl_token_free(token);
    rpl_tokenizer_set_mark(tokenizer, mark);
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
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_strbuffer_t buf = NULL;
    bool appended = false;

    size_t mark = rpl_tokenizer_get_mark(tokenizer);

    /* Reject program start glyph ('«', 0xC2 0xAB). */
    char first_bytes[3] = { 0 };
    first_bytes[0] = rpl_tokenizer_get_char(tokenizer);
    if (first_bytes[0] == (char)0xC2) {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    first_bytes[1] = rpl_tokenizer_get_char(tokenizer);
	}

	if (first_bytes[0] == (char)0xAB) goto back_out;
    }

    /* Accumulate what's already been read. */

    buf = rpl_strbuffer_new_empty(8);
    if (buf == NULL) goto back_out;

    appended = rpl_strbuffer_append_chars(buf, first_bytes);
    if (appended == false) goto back_out;

    /*
     Read every subsequent byte until a whitespace character or a
     delimiter (both of which end the token), or until there are no more
     characters (which must be backed out from).

     Note that one pair of Unicode delimiters ('«' and '»') require a
     little more lookahead than usual.
     */
    bool complete = false;
    int idx = 0;
    while ((complete == false) && rpl_tokenizer_has_char(tokenizer)) {
	char chs[3] = { 0 };
	chs[idx] = rpl_tokenizer_get_char(tokenizer);

	if ((idx == 0) && (rpl_char_is_whitespace(chs[idx])
			   || rpl_char_is_delimiter(chs[idx])))
	{
	    complete = true;
	} else if ((idx == 0) && (chs[0] == (char)0xC2)) {
	    /* Potential unicode delimiter. */
	    idx += 1;
	} else if ((idx == 1) && rpl_chars_are_unicode_delimiter(chs)) {
	    rpl_tokenizer_unget_char(tokenizer, chs[2]);

	    complete = true;
	    idx = 0;
	} else if (idx == 0) {
	    appended = rpl_strbuffer_append_chars(buf, chs);
	    if (appended == false) goto back_out;
	}
    }

    if (complete == false) goto back_out;

    token = rpl_token_new(rpl_token_type_identifier,
			  rpl_strbuffer_get_chars(buf),
			  NULL);

    rpl_strbuffer_free(buf);

    return token;

back_out:
    if (buf) rpl_strbuffer_free(buf);
    if (token) rpl_token_free(token);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}


RPL_SOURCE_END
