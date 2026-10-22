#!/bin/sh

# A quick script to pass the right args to make to build on NetBSD.

make -j 8 \
	OTHER_CFLAGS="-I /usr/pkg/include" \
	OTHER_LDFLAGS="-L/usr/pkg/lib -Wl,-rpath -Wl,/usr/pkg/lib" \
	ICU_LDFLAGS=-licutu
