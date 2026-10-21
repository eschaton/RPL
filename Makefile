#
#  Makefile
#  RPL
#
#  Created by Chris Hanson on 10/16/26.
#  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
#

.POSIX:

.SUFFIXES: .c .h

.INCLUDES: .h

.PATH:	librpl \
	librpl/execution \
	librpl/execution/operations \
	librpl/parsing \
	librpl/utilities \
	librpl/values \
	RPL

OBJECTS_librpl = \
	rpl_arithmetic_ops.o \
	rpl_cflow_ops.o \
	rpl_meta_ops.o \
	rpl_operations.o \
	rpl_stack_ops.o \
	rpl_variable_ops.o \
	rpl_context.o \
	rpl_environment.o \
	rpl_interpreter_defaults.o \
	rpl_interpreter.o \
	rpl_operation_table.o \
	rpl_operation.o \
	rpl_scope.o \
	rpl_stack.o \
	rpl_keyword.o \
	rpl_tokenizer.o \
	rpl_adjbuffer.o \
	rpl_stream.o \
	rpl_unicode.o \
	rpl_unistring.o \
	rpl_array.o \
	rpl_complex.o \
	rpl_identifier.o \
	rpl_integer.o \
	rpl_list.o \
	rpl_name.o \
	rpl_program.o \
	rpl_real.o \
	rpl_string.o \
	rpl_tagged.o \
	rpl_unit.o \
	rpl_value.o

OBJECTS_rpltool = \
	main.o


RPL_INCLUDES = \
	-I librpl \
	-I librpl/execution \
	-I librpl/execution/operations \
	-I librpl/parsing \
	-I librpl/utilities \
	-I librpl/values \
	-I RPL


CFLAGS += ${RPL_INCLUDES}

LDFLAGS += -ledit -liconv -licucore

all: rpltool

clean:
	rm -f ${OBJECTS_librpl} ${OBJECTS_rpltool} rpltool

librpl.a: ${OBJECTS_librpl}
	libtool -static ${OBJECTS_librpl} -o $@

rpltool: librpl.a ${OBJECTS_rpltool}
	${CC} ${LDFLAGS} librpl.a ${OBJECTS_rpltool} -o rpltool
