
MAN=

.include <bsd.own.mk>

PROG=	iwndebug

BINDIR?=/usr/local/sbin

CFLAGS+= -DIWN_DEBUG -I/usr/src/sys/ -Wno-unused-parameter

SRCS=	main.c

CFLAGS.clang+=	-fbracket-depth=512

.include <bsd.prog.mk>
