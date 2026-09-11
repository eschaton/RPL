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

#include "rpl_integer.h"
#include "rpl_name.h"
#include "rpl_string.h"
#include "rpl_token_internal.h"
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

const rpl_unichar_t rpl_unichar_space = ' ';
const rpl_unichar_t rpl_unichar_tab = '\t';
const rpl_unichar_t rpl_unichar_linefeed = '\n';
const rpl_unichar_t rpl_unichar_carriage_return = '\r';
const rpl_unichar_t rpl_unichar_single_quote = '\'';
const rpl_unichar_t rpl_unichar_double_quote = '"';
const rpl_unichar_t rpl_unichar_backslash = '\\';
const rpl_unichar_t rpl_unichar_plus = '+';
const rpl_unichar_t rpl_unichar_minus = '-';
const rpl_unichar_t rpl_unichar_period = '.';
const rpl_unichar_t rpl_unichar_comma = ',';
const rpl_unichar_t rpl_unichar_at = '@';
const rpl_unichar_t rpl_unichar_octothorpe = '#';
const rpl_unichar_t rpl_unichar_colon = ':';
const rpl_unichar_t rpl_unichar_underscore = '_';
const rpl_unichar_t rpl_unichar_parenthesis_open = '(';
const rpl_unichar_t rpl_unichar_parenthesis_close = ')';
const rpl_unichar_t rpl_unichar_bracket_open = '[';
const rpl_unichar_t rpl_unichar_bracket_close = ']';
const rpl_unichar_t rpl_unichar_brace_open = '{';
const rpl_unichar_t rpl_unichar_brace_close = '}';
const rpl_unichar_t rpl_unichar_chevron_open = 0x000AB;
const rpl_unichar_t rpl_unichar_chevron_close = 0x000BB;
const rpl_unichar_t rpl_unichar_measured_angle = 0x02221;
const rpl_unichar_t rpl_unichar_spherical_angle = 0x02222;

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
    if (token == NULL) goto back_out;

    rpl_value_release(value); /* owned by token now */

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
    // TODO: Implement rpl_tokenizer_tokenize_program
    return NULL;
}

rpl_token_t RPL_NULLABLE
rpl_tokenizer_tokenize_string(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    rpl_token_t token = NULL;
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

    rpl_value_t value = rpl_string_new(buf);
    if (value == NULL) goto back_out;

    token = rpl_token_new(rpl_token_type_value, NULL, value);
    if (token == NULL) goto back_out;

    rpl_value_release(value); /* owned by token now */

    rpl_unistring_release(buf);

    return token;

back_out:
    if (token) rpl_token_free(token);
    if (buf) rpl_unistring_release(buf);
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
	    token = rpl_tokenizer_tokenize_identifier(tokenizer);
	}
    }

    return token;
}


RPL_SOURCE_END
