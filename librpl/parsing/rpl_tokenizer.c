//
//  rpl_tokenizer.c
//  librpl
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_tokenizer_internal.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_keyword.h"
#include "rpl_token_internal.h"

#include "rpl_identifier.h"
#include "rpl_integer.h"
#include "rpl_name.h"
#include "rpl_program_internal.h"
#include "rpl_string.h"
#include "rpl_unit.h"


RPL_SOURCE_BEGIN


rpl_tokenizer_t RPL_NULLABLE
rpl_tokenizer_new(rpl_context_t context)
{
    assert(context != NULL);

    rpl_tokenizer_t tokenizer = calloc(1, sizeof(struct rpl_tokenizer));
    if (tokenizer) {
	tokenizer->_context = context;
	tokenizer->_strbuffer = rpl_unistring_new(256);
	if (tokenizer->_strbuffer == NULL) goto error;
	tokenizer->_cur = -1;
    }

    return tokenizer;

error:
    rpl_tokenizer_free(tokenizer);
    return NULL;
}

void
rpl_tokenizer_free(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    if (tokenizer->_strbuffer) {
	rpl_unistring_release(tokenizer->_strbuffer);
    }

    free(tokenizer);
}

bool
rpl_tokenizer_append(rpl_tokenizer_t tokenizer, rpl_unistring_t str)
{
    assert(tokenizer != NULL);
    assert(str != NULL);
    assert(rpl_unistring_get_length(str) > 0);

    rpl_unistring_t sb = tokenizer->_strbuffer;

    if (tokenizer->_cur == rpl_unistring_get_length(sb)) {
	/*
	 If the buffer has been entirely consumed, reset it and the
	 tokenizer.
	 */

	rpl_unistring_remove_all(sb);
	tokenizer->_cur = 0;
    }

    if (rpl_unistring_append(tokenizer->_strbuffer, str)) {
	/*
	 If the append succeeds and there's no current index yet, start
	 the current index at 0.
	 */

	if (tokenizer->_cur == -1) tokenizer->_cur = 0;
	return true;
    } else {
	return false;
    }
}

rpl_unichar_t
rpl_tokenizer_get_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_unichar_t ch;
    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unistring_t sb = tokenizer->_strbuffer;
	ch = rpl_unistring_get_char(sb, tokenizer->_cur);
	tokenizer->_cur += 1;
    } else {
	ch = '\0';
    }

    return ch;
}

void
rpl_tokenizer_unget_char(rpl_tokenizer_t tokenizer, rpl_unichar_t ch)
{
    assert(tokenizer != NULL);
    assert(tokenizer->_cur > 0);
    assert(ch != '\0');

    rpl_unistring_t sb = tokenizer->_strbuffer;
    tokenizer->_cur -= 1;

    assert(rpl_unistring_get_char(sb, tokenizer->_cur) == ch);
}

rpl_unichar_t
rpl_tokenizer_peek_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_unichar_t ch = '\0';

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unistring_t sb = tokenizer->_strbuffer;
	ch = rpl_unistring_get_char(sb, tokenizer->_cur);
    }

    return ch;
}

bool
rpl_tokenizer_has_char(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    if (tokenizer->_cur != -1) {
	const size_t length
	    = rpl_unistring_get_length(tokenizer->_strbuffer);
	return (tokenizer->_cur < length);
    } else {
	return false;
    }
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
rpl_char_is_end_of_line(rpl_unichar_t ch)
{
    return ((ch == rpl_unichar_linefeed)
	    || (ch == rpl_unichar_carriage_return));
}

bool
rpl_char_is_whitespace(rpl_unichar_t ch)
{
    return ((ch == rpl_unichar_space)
	    || (ch == rpl_unichar_tab)
	    || rpl_char_is_end_of_line(ch));
}

bool
rpl_char_is_delimiter(rpl_unichar_t ch)
{
    return ((ch == rpl_unichar_parenthesis_open)
	    || (ch == rpl_unichar_parenthesis_close))
	|| ((ch == rpl_unichar_bracket_open)
	    || (ch == rpl_unichar_bracket_close))
	|| ((ch == rpl_unichar_brace_open)
	    || (ch == rpl_unichar_brace_close))
	|| ((ch == rpl_unichar_chevron_open)
	    || (ch == rpl_unichar_chevron_close))
	|| (ch == rpl_unichar_double_quote)
	|| (ch == rpl_unichar_single_quote);
}

bool
rpl_char_is_comment_start(rpl_unichar_t ch)
{
    return (ch == rpl_unichar_at);
}

bool
rpl_char_is_integer_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_octothorpe;
}

bool
rpl_char_is_digit(rpl_unichar_t ch)
{
    return isdigit(ch);
}

bool
rpl_char_is_hexdigit(rpl_unichar_t ch)
{
    return rpl_char_is_digit(ch)
	|| ((ch >= 'A') && (ch <= 'F'))
	|| ((ch >= 'a') && (ch <= 'f'));
}

int
rpl_char_digit_value(rpl_unichar_t ch)
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
rpl_char_is_base_indicator(rpl_unichar_t ch)
{
    return (ch == 'b') || (ch == 'd') || (ch == 'h') || (ch == 'o');
}

/*! A real number can start with a sign, a digit, or a decimal point. */
bool
rpl_char_is_real_start(rpl_unichar_t ch)
{
    return ((ch == rpl_unichar_plus) || (ch == rpl_unichar_minus))
	|| (ch == rpl_unichar_period)
	|| rpl_char_is_digit(ch);
}

bool
rpl_char_is_complex_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_parenthesis_open;
}

bool
rpl_char_is_array_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_bracket_open;
}

bool
rpl_char_is_name_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_single_quote;
}

bool
rpl_char_is_program_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_chevron_open;
}

bool
rpl_char_is_string_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_double_quote;
}

bool
rpl_char_is_list_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_brace_open;
}

bool
rpl_char_is_tagged_start(rpl_unichar_t ch)
{
    return ch == rpl_unichar_colon;
}

bool
rpl_char_is_identifier_start(rpl_unichar_t ch)
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
	rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
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

    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
    assert(rpl_char_is_comment_start(ch));

    bool saw_eol = false;
    while (rpl_tokenizer_has_char(tokenizer)) {
	ch = rpl_tokenizer_get_char(tokenizer);
	if (rpl_char_is_end_of_line(ch)) {
	    saw_eol = true;
	    break;
	}
    }

    /* We don't have a complete line, rewind. */

    if (saw_eol == false) {
	rpl_tokenizer_set_mark(tokenizer, mark);
    }
}

/*!
 Return a buffer of name text, if possible.

 Name text is any sequence of characters that aren't whitspace or
 delimiters, that also doesn't start with a digit.
 */
rpl_unistring_t RPL_NULLABLE
rpl_tokenizer_copy_name_text(rpl_tokenizer_t tokenizer)
RPL_RETURNS_RETAINED
{
    assert(tokenizer != NULL);

    rpl_unistring_t buf = NULL;
    bool appended = false;

    enum parser_state {
	parser_state_start = 0,
	parser_state_accumulating,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_unistring_new(8);
    if (buf == NULL) goto back_out;

    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    if (!rpl_char_is_whitespace(ch)
			&& !rpl_char_is_delimiter(ch)
			&& !rpl_char_is_digit(ch))
		    {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			state = parser_state_accumulating;
		    } else {
			rpl_tokenizer_unget_char(tokenizer, ch);
			state = parser_state_end;
		    }
		} break;

		case parser_state_accumulating: {
		    if (!rpl_char_is_whitespace(ch)
			&& !rpl_char_is_delimiter(ch))
		    {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
		    } else {
			rpl_tokenizer_unget_char(tokenizer, ch);
			state = parser_state_end;
		    }
		} break;

		case parser_state_end: {
		    /*
		     Shouldn't actually get here, the loop should exit.
		     */
		} break;
	    }
	} else {
	    /* Break out of loop, no matter what's been parsed. */
	    state = parser_state_end;
	}
    } while (state != parser_state_end);

    /* Do not allow empty name_text. */

    if (rpl_unistring_get_length(buf) == 0) goto back_out;

    return buf;

back_out:
    if (buf) rpl_unistring_release(buf);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

/*!
 Return a real value for the next token, if possible.

 Real numbers use the syntax

     real := /[+-]?[0-9]*(.[0-9]*)?([eE][+-][0-9]+)?/.

 or, to describe it in prose describing more nuance than the above
 allows:

 - An optional sign;
 - Optional while digits with an optional decimal point followed by zero
   or more fractional digits;
 - An optional exponent marker and exponent digits.

 */
rpl_value_t RPL_NULLABLE
rpl_tokenizer_copy_real_value(rpl_tokenizer_t tokenizer)
RPL_RETURNS_RETAINED
{
    assert(tokenizer != NULL);

    rpl_value_t value = NULL;
    rpl_unistring_t buf = NULL;
    bool complete = false;
    bool appended;

    enum parser_state {
	parser_state_start = 0,
	parser_state_sign,
	parser_state_whole_part,
	parser_state_decimal_point,
	parser_state_fractional_part,
	parser_state_exponent_marker,
	parser_state_exponent_sign,
	parser_state_exponent_part,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_unistring_new(8);
    if (buf == NULL) goto back_out;

    int whole_digit_count = -1;
    int fractional_digit_count = -1;
    int exponent_digit_count = -1;
    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    if ((ch == rpl_unichar_plus)
			|| (ch == rpl_unichar_minus))
		    {
			state = parser_state_sign;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else if (rpl_char_is_digit(ch)) {
			state = parser_state_whole_part;
			whole_digit_count = 0;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else if (ch == rpl_unichar_period) {
			state = parser_state_decimal_point;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else {
			goto back_out;
		    }
		} break;

		case parser_state_sign: {
		    if ((ch == rpl_unichar_plus)
			|| (ch == rpl_unichar_minus))
		    {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			state = parser_state_whole_part;
			whole_digit_count = 0;
		    } else {
			goto back_out;
		    }
		} break;

		case parser_state_whole_part: {
		    if (rpl_char_is_digit(ch)) {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			whole_digit_count += 1;
		    } else if (ch == rpl_unichar_period) {
			state = parser_state_decimal_point;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else if ((ch == 'E') || (ch == 'e')) {
			state = parser_state_exponent_marker;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else {
			complete = whole_digit_count > 0;
			state = parser_state_end;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		} break;

		case parser_state_decimal_point: {
		    assert(ch == rpl_unichar_period);
		    appended = rpl_unistring_append_char(buf, ch);
		    if (appended == false) goto back_out;
		    fractional_digit_count = 0;
		    state = parser_state_fractional_part;
		} break;

		case parser_state_fractional_part: {
		    if (rpl_char_is_digit(ch)) {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			fractional_digit_count += 1;
		    } else if ((ch == 'E') || (ch == 'e')) {
			state = parser_state_exponent_marker;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else {
			complete = fractional_digit_count > 0;
			state = parser_state_end;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		} break;

		case parser_state_exponent_marker: {
		    assert((ch == 'E') || (ch == 'e'));
		    appended = rpl_unistring_append_char(buf, ch);
		    if (appended == false) goto back_out;
		    state = parser_state_exponent_sign;
		} break;

		case parser_state_exponent_sign: {
		    if ((ch == rpl_unichar_plus)
			|| (ch == rpl_unichar_minus))
		    {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			exponent_digit_count = 0;
			state = parser_state_exponent_part;
		    } else if (rpl_char_is_digit(ch)) {
			exponent_digit_count = 0;
			state = parser_state_exponent_part;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    } else {
			/*
			 Not complete, something must come after 'E'
			 (within the real) before another token.
			 */
			state = parser_state_end;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		} break;

		case parser_state_exponent_part: {
		    if (rpl_char_is_digit(ch)) {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
			exponent_digit_count += 1;
		    } else {
			complete = exponent_digit_count > 0;
			state = parser_state_end;
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		} break;

		case parser_state_end: {
		    /*
		     Shouldn't actually get here, the loop should exit.
		     */
		} break;
	    }
	} else {
	    /* Break out of loop, no matter what's been parsed. */
	    state = parser_state_end;
	}
    } while (state != parser_state_end);

    if (!complete) goto back_out;

    /* buf contains the textual form of a real number */

    char *buf_utf8 = rpl_unistring_copy_utf8(buf);
    if (buf_utf8 == NULL) goto back_out;
    rpl_real_t rep = strtod(buf_utf8, NULL);
    free(buf_utf8);

    value = rpl_real_new(rep);
    if (value == NULL) goto back_out;

    rpl_unistring_release(buf);

    return value;

back_out:
    if (buf) rpl_unistring_release(buf);
    if (value) rpl_value_release(value);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

/*!
 Tokenize a binary integer.

 Binary integers use the syntax

     integer := '#' ' '? /[0-9A-Fa-f]+[bdoh]?/.

 where _one space_ is allowed (but not required) between the `'#'` and
 the digits, and the allowed digits depend on either the current
 environment's integer base or the optional suffix.
 */
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_integer(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_unistring_t buf = NULL;
    bool appended = false;

    enum parser_state {
	parser_state_start = 0,
	parser_state_saw_octothorpe,
	parser_state_accumulating_digits,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_unistring_new(8);
    if (buf == NULL) goto back_out;

    int max_digit = -1;
    rpl_unichar_t base_indicator = '\0';

    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    assert(ch == rpl_unichar_octothorpe);
		    state = parser_state_saw_octothorpe;
		} break;

		case parser_state_saw_octothorpe: {
		    if (ch != rpl_unichar_space) {
			/* Skip exactly one space. */
			rpl_tokenizer_unget_char(tokenizer, ch);
		    }
		    state = parser_state_accumulating_digits;
		} break;

		case parser_state_accumulating_digits: {
		    if (rpl_char_is_hexdigit(ch)) {
			appended = rpl_unistring_append_char(buf, ch);
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
	} else {
	    /* Break out of loop, no matter what's been parsed. */
	    state = parser_state_end;
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

    char *buf_utf8 = rpl_unistring_copy_utf8(buf);
    if (buf_utf8 == NULL) goto back_out;
    rpl_integer_t rep = strtoull(buf_utf8, NULL, base);
    free(buf_utf8);

    rpl_value_t value = rpl_integer_new(rep);
    if (value == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, value);
    rpl_value_release(value); /* owned by token now */
    if (token == NULL) goto back_out;

    rpl_unistring_release(buf);

    return token;

back_out:
    if (buf) rpl_unistring_release(buf);
    if (token) rpl_token_free(token);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_real_or_unit(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    rpl_value_t real_part = NULL;
    rpl_unistring_t unit_text = NULL;
    rpl_value_t unit_part = NULL;
    rpl_value_t value = NULL;

    real_part = rpl_tokenizer_copy_real_value(tokenizer);
    if (real_part == NULL) goto back_out;

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	if (ch == rpl_unichar_underscore) {
	    unit_text = rpl_tokenizer_copy_name_text(tokenizer);
	    if (unit_text == NULL) goto back_out;

	    unit_part = rpl_name_new(unit_text);
	    if (unit_part == NULL) goto back_out;

	    value = rpl_unit_new(rpl_real_get_rep(real_part),
				 unit_part);
	    if (value == NULL) goto back_out;
	} else {
	    rpl_tokenizer_unget_char(tokenizer, ch);

	    value = real_part;
	    real_part = NULL;
	}
    }

    if (value) {
	token = rpl_token_new(rpl_token_type_value, NULL, value);
	if (token == NULL) goto back_out;
	rpl_value_release(value);
    }

    if (real_part) rpl_value_release(real_part);
    if (unit_text) rpl_unistring_release(unit_text);
    if (unit_part) rpl_value_release(unit_part);

    return token;

back_out:
    if (real_part) rpl_value_release(real_part);
    if (unit_text) rpl_unistring_release(unit_text);
    if (unit_part) rpl_value_release(unit_part);
    if (value) rpl_value_release(value);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_complex(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_complex
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_array(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_array
    return NULL;
}

/*!
 Tokenize a name.

 Names use the syntax

     name_text = character+.
     name = '\'' name_text '\''.

 with the constraint that `name_text` does not allow whitespace or
 delimiters.
 */
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_name(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_unistring_t buf = NULL;
    rpl_value_t value = NULL;

    ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t open_quote = rpl_tokenizer_get_char(tokenizer);
	if (open_quote != rpl_unichar_single_quote) goto back_out;
    } else {
	goto back_out;
    }

    buf = rpl_tokenizer_copy_name_text(tokenizer);
    if (buf == NULL) goto back_out;

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t close_quote = rpl_tokenizer_get_char(tokenizer);
	if (close_quote != rpl_unichar_single_quote) goto back_out;
    } else {
	goto back_out;
    }

    value = rpl_name_new(buf);
    if (value == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, value);
    if (token == NULL) goto back_out;

    rpl_value_release(value); /* owned by token now */

    rpl_unistring_release(buf);

    return token;

back_out:
    if (value) rpl_value_release(value);
    if (token) rpl_token_free(token);
    if (buf) rpl_unistring_release(buf);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_program(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_value_t program = NULL;
    rpl_token_t next = NULL;
    rpl_value_t value = NULL;
    rpl_unistring_t identifier = NULL;
    bool complete = false;

    enum parser_state {
	parser_state_start = 0,
	parser_state_accumulating_tokens,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    program = rpl_program_new();
    if (program == NULL) goto back_out;

    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    if (ch == rpl_unichar_chevron_open) {
			state = parser_state_accumulating_tokens;
		    } else {
			rpl_tokenizer_unget_char(tokenizer, ch);
			goto back_out;
		    }
		} break;

		case parser_state_accumulating_tokens: {
		    /*
		     Get tokens until a `»` is seen in the input stream.
		     Since any program in the input stream will show up
		     as a single token here (as this is a recursive
		     descent parser), nesting is handled automatically.
		     */
		    if (ch == rpl_unichar_chevron_close) {
			complete = true;
			state = parser_state_end;
		    } else if (rpl_char_is_whitespace(ch)) {
			/* Just skip whitespace characters. */
		    } else {
			rpl_tokenizer_unget_char(tokenizer, ch);

			next = rpl_tokenizer_copy_next(tokenizer);
			if (next == NULL) goto back_out;

			switch (rpl_token_get_type(next)) {
			    case rpl_token_type_value: {
				value = rpl_token_get_value(next);
				assert(value != NULL);

				bool appended
				    = rpl_program_append(program,
							 value);
				if (appended == false) goto back_out;
			    } break;

			    case rpl_token_type_identifier: {
				identifier = rpl_token_get_string(next);
				assert(identifier != NULL);

				value = rpl_identifier_new(identifier);
				if (value == NULL) goto back_out;

				bool appended
				    = rpl_program_append(program,
							 value);
				if (appended == false) goto back_out;
			    } break;
			}

			rpl_token_free(next); next = NULL;
			value = NULL;
		    }
		} break;

		case parser_state_end: {
		    /*
		     Shouldn't actually get here, the loop should exit.
		     */
		} break;
	    }
	} else {
	    /* Break out of loop, no matter what's been parsed. */
	    state = parser_state_end;
	}
    } while (state != parser_state_end);

    /* Back out if the program is incomplete. */

    if (complete == false) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, program);
    if (token == NULL) goto back_out;

    rpl_value_release(program); /* owned by token now */

    return token;

back_out:
    if (token) rpl_token_free(token);
    if (program) rpl_value_release(program);
    if (next) rpl_token_free(next);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_string(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_value_t value = NULL;
    rpl_unistring_t buf = NULL;
    bool appended = false;

    enum parser_state {
	parser_state_start = 0,
	parser_state_accumulating_content,
	parser_state_saw_escape,
	parser_state_end,
    } state = parser_state_start;

    const ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_unistring_new(8);
    if (buf == NULL) goto back_out;

    do {
	if (rpl_tokenizer_has_char(tokenizer)) {
	    rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	    switch (state) {
		case parser_state_start: {
		    if (ch == rpl_unichar_double_quote) {
			state = parser_state_accumulating_content;
		    } else {
			rpl_tokenizer_unget_char(tokenizer, ch);
			goto back_out;
		    }
		} break;
		case parser_state_accumulating_content: {
		    if (ch == rpl_unichar_backslash) {
			state = parser_state_saw_escape;
		    } else if (ch == rpl_unichar_double_quote) {
			state = parser_state_end;
		    } else {
			appended = rpl_unistring_append_char(buf, ch);
			if (appended == false) goto back_out;
		    }
		} break;
		case parser_state_saw_escape: {
		    rpl_unichar_t to_append = 0;
		    switch (ch) {
			case 'n':
			    to_append = rpl_unichar_linefeed;
			    break;
			case 'r':
			    to_append = rpl_unichar_carriage_return;
			    break;
			case 't':
			    to_append = rpl_unichar_tab;
			    break;
			case rpl_unichar_backslash:
			case rpl_unichar_double_quote:
			    to_append = ch;
			    break;
		    }

		    /* Handle invalid syntax. */
		    if (to_append == 0) goto back_out;

		    appended
			= rpl_unistring_append_char(buf, to_append);
		    if (appended == false) goto back_out;
		    state = parser_state_accumulating_content;
		} break;
		case parser_state_end: {
		    /*
		     Shouldn't actually get here, the loop should exit.
		     */
		} break;
	    }
	} else {
	    /* Break out of loop, no matter what's been parsed. */
	    state = parser_state_end;
	}
    } while (state != parser_state_end);

    value = rpl_string_new(buf);
    if (value == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, value);
    if (token == NULL) goto back_out;

    rpl_value_release(value); /* owned by token now */

    rpl_unistring_release(buf);

    return token;

back_out:
    if (token) rpl_token_free(token);
    if (buf) rpl_unistring_release(buf);
    if (value) rpl_value_release(value);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_list(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_list
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_tagged(rpl_tokenizer_t tokenizer)
{
    // TODO: Implement rpl_tokenizer_tokenize_tagged
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_identifier(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
    rpl_unistring_t buf = NULL;
    bool appended = false;

    size_t mark = rpl_tokenizer_get_mark(tokenizer);

    buf = rpl_unistring_new(8);
    if (buf == NULL) goto back_out;

    /*
     Read every subsequent character until either a whitespace character
     or a delimiter (both of which end the token), or until there are no
     more characters to read (which must be backed out from).
     */
    bool complete = false;
    while ((complete == false) && rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t ch = rpl_tokenizer_get_char(tokenizer);
	if (!rpl_char_is_whitespace(ch)
	    && !rpl_char_is_delimiter(ch))
	{
	    appended = rpl_unistring_append_char(buf, ch);
	    if (appended == false) goto back_out;
	} else {
	    rpl_tokenizer_unget_char(tokenizer, ch);
	    complete = true;
	}
    }

    if (complete == false) goto back_out;

    token = rpl_token_new(rpl_token_type_identifier, buf, NULL);
    if (token == NULL) goto back_out;

    rpl_unistring_release(buf);

    return token;

back_out:
    if (buf) rpl_unistring_release(buf);
    if (token) rpl_token_free(token);
    rpl_tokenizer_set_mark(tokenizer, mark);
    return NULL;
}

bool
rpl_token_is_control_flow_start(rpl_token_t token,
				rpl_program_type_t *type)
{
    assert(token != NULL);
    assert(type != NULL);

    /* Non-identifier tokens can't start control flow. */
    if (rpl_token_get_type(token) != rpl_token_type_identifier) {
	return false;
    }

    rpl_unistring_t identifier = rpl_token_get_string(token);
    assert(identifier != NULL);

    /* All control-flow identifiers are 2-5 characters in length. */
    const size_t ident_len = rpl_unistring_get_length(identifier);
    if ((ident_len < 2) || (ident_len > 5)) return false;

    /* Search the set of control-flow identifiers for a match. */
    struct cflow_start {
	rpl_unistring_t str;
	rpl_program_type_t type;
    } starts[] = {
	{ rpl_keyword_DO(),    rpl_program_type_DO },
	{ rpl_keyword_IF(),    rpl_program_type_IF },
	{ rpl_keyword_FOR(),   rpl_program_type_FOR },
	{ rpl_keyword_CASE(),  rpl_program_type_CASE },
	{ rpl_keyword_START(), rpl_program_type_START },
	{ rpl_keyword_WHILE(), rpl_program_type_WHILE },
	{ NULL,                rpl_program_type_generic },
    };

    bool found = false;
    for (struct cflow_start *start = &starts[0];
	 (start->str != NULL) && !found;
	 start++)
    {
	if (rpl_unistring_is_equal_case_insensitive(identifier,
						    start->str))
	{
	    *type = start->type;
	    found = true;
	}
    }

    return found;

error:
    return false;
}

bool
rpl_tokenizer_is_keyword(rpl_unistring_t ident, const char *keyword)
{
    assert(ident != NULL);
    assert(keyword != NULL);

    return rpl_unistring_is_equal_case_insensitive_utf8(ident, keyword);
}

/*!
 Parse a `DO ... UNTIL ... END` construct into a program.

 At this point, the tokenizer has already consumed the `DO` identifier,
 and has saved a mark, so it's sufficient to return `NULL` to back out.

 The construct is transformed from

     DO loop-clause UNTIL test-clause END

 to

     « « loop-clause » « test-clause » _DO »

 so it can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `_DO` operation itself.
 */
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_DO(rpl_tokenizer_t tokenizer)
{
    // TODO: parse DO
    return NULL;
}

/*!
 Parse an `IF ... THEN ... [ELSE ...] END` construct into a program.

 At this point, the tokenizer has already consumed the `IF` identifier,
 and has saved a mark, so it's sufficient to return `NULL` to back out.

 The construct is transformed from

     IF test THEN then-clause END

 into

     « « test » « then-clause » IFT »

 and from

     IF test THEN then-clause ELSE else-clause END

 into

     « « test » « then-clause » « else-clause » IFTE »

 so they can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `IFT` and `IFTE`
 operations themselves.
 */
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_IF(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    enum parser_state {
	parser_state_start = 0,
	parser_state_then,
	parser_state_else,
	parser_state_end,
    } state = parser_state_start;

    rpl_value_t program = NULL;
    rpl_value_t test_subp = NULL;
    rpl_value_t then_subp = NULL;
    rpl_value_t else_subp = NULL;
    rpl_token_t token = NULL;
    bool appended;

    program = rpl_program_new_with_type(rpl_program_type_IF);
    if (program == NULL) goto back_out;

    test_subp
	= rpl_program_new_with_type(rpl_program_type_intermediate);
    if (test_subp == NULL) goto back_out;

    then_subp
	= rpl_program_new_with_type(rpl_program_type_intermediate);
    if (then_subp == NULL) goto back_out;

    else_subp
	= rpl_program_new_with_type(rpl_program_type_intermediate);
    if (else_subp == NULL) goto back_out;

    do {
	token = rpl_tokenizer_copy_next(tokenizer);
	if (token == NULL) goto back_out;

	rpl_token_type_t type = rpl_token_get_type(token);
	rpl_value_t value = NULL;
	rpl_unistring_t ident = NULL;

	if (type == rpl_token_type_value) {
	    value = rpl_token_get_value(token);
	} else {
	    ident = rpl_token_get_string(token);
	}

	switch (state) {
	    case parser_state_start: {
		/*
		 Until a `THEN` is seen, accumulate to the "test"
		 subprogram.
		 */

		if (ident && rpl_tokenizer_is_keyword(ident, "THEN")) {
		    state = parser_state_then;
		} else {
		    if (value) {
			appended = rpl_program_append(test_subp, value);
		    } else if (ident) {
			rpl_value_t iv = rpl_identifier_new(ident);
			if (iv == NULL) goto back_out;

			appended = rpl_program_append(test_subp, iv);
			rpl_value_release(iv);
		    } else {
			/* Should never happen. */
			assert((ident != NULL) || (value != NULL));
			appended = true;
		    }
		    if (appended == false) goto back_out;
		}
	    } break;

	    case parser_state_then: {
		/*
		 Until an `ELSE` or `END` is seen, accumulate to the
		 "then" subprogram.
		 */

		if (ident && rpl_tokenizer_is_keyword(ident, "ELSE")) {
		    state = parser_state_else;
		} else if (ident && rpl_tokenizer_is_keyword(ident,
							     "END"))
		{
		    /* No "else" subprogram, clear it. */
		    rpl_value_release(else_subp); else_subp = NULL;

		    state = parser_state_end;
		} else {
		    if (value) {
			appended = rpl_program_append(then_subp, value);
		    } else if (ident) {
			rpl_value_t iv = rpl_identifier_new(ident);
			if (iv == NULL) goto back_out;

			appended = rpl_program_append(then_subp, iv);
			rpl_value_release(iv);
		    } else {
			/* Should never happen. */
			assert((ident != NULL) || (value != NULL));
			appended = true;
		    }
		    if (appended == false) goto back_out;
		}
	    } break;

	    case parser_state_else: {
		/*
		 Until an `END` is seen, accumulate to the "else"
		 subprogram.
		 */

		if (ident && rpl_tokenizer_is_keyword(ident, "END")) {
		    state = parser_state_end;
		} else {
		    if (value) {
			appended = rpl_program_append(else_subp, value);
		    } else if (ident) {
			rpl_value_t iv = rpl_identifier_new(ident);
			if (iv == NULL) goto back_out;

			appended = rpl_program_append(else_subp, iv);
			rpl_value_release(iv);
		    } else {
			/* Should never happen. */
			assert((ident != NULL) || (value != NULL));
			appended = true;
		    }
		    if (appended == false) goto back_out;
		}
	    } break;

	    case parser_state_end: {
		/* Shouldn't actually get here, the loop should exit. */
	    } break;
	}

	rpl_token_free(token); token = NULL;
    } while (state != parser_state_end);

    appended = rpl_program_append(program, test_subp);
    if (appended == false) goto back_out;

    appended = rpl_program_append(program, then_subp);
    if (appended == false) goto back_out;

    if (else_subp) {
	appended = rpl_program_append(program, else_subp);
	if (appended == false) goto back_out;

	rpl_unistring_t IFTE_str = rpl_unistring_new_from_utf8("IFTE",
							       4);
	if (IFTE_str == NULL) goto back_out;

	rpl_value_t IFTE = rpl_identifier_new(IFTE_str);
	rpl_unistring_release(IFTE_str);
	if (IFTE == NULL) goto back_out;

	appended = rpl_program_append(program, IFTE);
	rpl_value_release(IFTE);
	if (appended == false) goto back_out;
    } else {

	rpl_unistring_t IFT_str = rpl_unistring_new_from_utf8("IFT", 3);
	if (IFT_str == NULL) goto back_out;

	rpl_value_t IFT = rpl_identifier_new(IFT_str);
	rpl_unistring_release(IFT_str);
	if (IFT == NULL) goto back_out;

	appended = rpl_program_append(program, IFT);
	rpl_value_release(IFT);
	if (appended == false) goto back_out;
    }

    return program;

back_out:
    if (program) rpl_value_release(program);
    if (token) rpl_token_free(token);
    return NULL;
}

/*!
 Parse a `FOR ... NEXT|STEP` construct into a program.

 At this point, the tokenizer has already consumed the `FOR` identifier,
 and has saved a mark, so it's sufficient to return `NULL` to back out.

 The construct is transformed from

     start finish FOR counter loop-clause NEXT

 to

     « start finish « → counter « loop-clause » » _FORNEXT »

 and from

     start finish FOR counter loop-clause increment STEP

 to

     « start finish « → counter « loop-clause » » increment _FORSTEP »

 so they can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `_FORNEXT` and `_FORSTEP`
 operations themselves.
*/
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_FOR(rpl_tokenizer_t tokenizer)
{
    // TODO: parse FOR
    return NULL;
}

/*!
 Parse a `CASE {... THEN ... END} ... END` construct into a program.

 At this point, the tokenizer has already consumed the `CASE`
 identifier, and has saved a mark, so it's sufficient to return `NULL`
 to back out.

 The construct is transformed from

     CASE
       test-clause-1 THEN true-clause-1 END
       test-clause-2 THEN true-clause-2 END
       …
       test-clause-n
       [default-clause]
     END

 to

     «
       « test-clause-1 » « true-clause-1 »
       « test-clause-2 » « true-clause-2 »
       …
       « test-clause-n » « true-clause-n »
       n « default-clause » _CASE
     »

 so it can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `_CASE` operation itself.

 Note that the _default-clause_ is optional in the control-flow
 construct but not to the `_CASE` operation; if one isn't supplied in
 the control-flow construct, an empty program is passed in its place to
 `_CASE`.
 */
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_CASE(rpl_tokenizer_t tokenizer)
{
    // TODO: parse CASE
    return NULL;
}

/*!
 Parse a `START ... NEXT|STEP` construct into a program.

 At this point, the tokenizer has already consumed the `START`
 identifier, and has saved a mark, so it's sufficient to return `NULL`
 to back out.

 The construct is transformed from

     start finish START loop-clause NEXT

 to

     « start finish « loop-clause » _STARTNEXT »

 and from

     start finish START loop-clause increment STEP

 to

     « start finish « loop-clause » increment _STARTSTEP »

 so they can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `_STARTNEXT` and
 `_STARTSTEP` operations themselves.
*/
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_START(rpl_tokenizer_t tokenizer)
{
    // TODO: parse START
    return NULL;
}

/*!
 Parse a `WHILE ... REPEAT ... END` construct into a program.

 At this point, the tokenizer has already consumed the `WHILE`
 identifier, and has saved a mark, so it's sufficient to return `NULL`
 to back out.

 The construct is transformed from

     WHILE test-clause REPEAT loop-clause END

 to

     « « test-clause » « loop-clause » _WHILE »

 so it can be handled by the standard evaluation process, with any
 special evaluation rules implementable by the `_WHILE` operation
 itself.
 */
rpl_value_t RPL_NULLABLE
rpl_tokenizer_parse_WHILE(rpl_tokenizer_t tokenizer)
{
    // TODO: parse WHILE
    return NULL;
}

/*!
 Tokenize one of the several control-flow constructs.

 Several control-flow constructs in RPL have special syntax that doesn't
 fit neatly with the stack-based model. These are represented instead by
 creating equivalent programs, and labeling these programs with their
 associated construct so they can be printed in an equivalent form to
 how they were written.

 The constructs are:

     DO ... UNTIL ... END
     IF ... THEN ... [ELSE ...] END
     FOR ... NEXT|STEP
     CASE {... THEN ... END} [...] END
     START ... NEXT|STEP
     WHILE ... REPEAT ... END

 Each one has its own tokenizer that produces a program.
 */
rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_control_flow(rpl_tokenizer_t tokenizer,
				    rpl_token_t start_token,
				    rpl_program_type_t type)
{
    assert(tokenizer != NULL);
    assert(start_token != NULL);

    rpl_token_t token = NULL;
    rpl_value_t program = NULL;

    switch (type) {
	case rpl_program_type_generic:
	case rpl_program_type_intermediate:
	    assert(false); /* These should never happen. */
	    break;

	case rpl_program_type_DO:
	    program = rpl_tokenizer_parse_DO(tokenizer);
	    break;

	case rpl_program_type_IF:
	    program = rpl_tokenizer_parse_IF(tokenizer);
	    break;

	case rpl_program_type_FOR:
	    program = rpl_tokenizer_parse_FOR(tokenizer);
	    break;

	case rpl_program_type_CASE:
	    program = rpl_tokenizer_parse_CASE(tokenizer);
	    break;

	case rpl_program_type_START:
	    program = rpl_tokenizer_parse_START(tokenizer);
	    break;

	case rpl_program_type_WHILE:
	    program = rpl_tokenizer_parse_WHILE(tokenizer);
	    break;
    }

    if (program == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, program);
    // TODO: Signal 'resource exhaustion' condition
    if (token == NULL) goto back_out;

    rpl_value_release(program); /* owned by token now */

    return token;

back_out:
    if (token) rpl_token_free(token);
    if (program) rpl_value_release(program);
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_copy_next(rpl_tokenizer_t tokenizer)
{
    rpl_token_t token = NULL;

    /* Skip whitespace and comments. */

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t ch = rpl_tokenizer_peek_char(tokenizer);

	if (rpl_char_is_whitespace(ch)) {
	    rpl_tokenizer_skip_whitespace(tokenizer);
	} else if (rpl_char_is_comment_start(ch)) {
	    rpl_tokenizer_skip_comment(tokenizer);
	}
    }

    /* Figure out what comes next. */

    if (rpl_tokenizer_has_char(tokenizer)) {
	rpl_unichar_t ch = rpl_tokenizer_peek_char(tokenizer);

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

	if ((token == NULL) && rpl_char_is_identifier_start(ch)) {
	    ssize_t mark = rpl_tokenizer_get_mark(tokenizer);

	    token = rpl_tokenizer_tokenize_identifier(tokenizer);
	    if (token) {
		/*
		 Some identifiers are special and introduce a control
		 flow construct, which is represented as a program.
		 */
		rpl_program_type_t type;
		if (rpl_token_is_control_flow_start(token, &type)) {
		    token
			= rpl_tokenizer_tokenize_control_flow(tokenizer,
							      token,
							      type);
		    if (token == NULL) {
			/*
			 Upon failure to parse a complete control-flow
			 construct, the mark must be reset.
			 */
			rpl_tokenizer_set_mark(tokenizer, mark);
		    }
		}
	    }
	}
    }

    return token;
}


RPL_SOURCE_END
