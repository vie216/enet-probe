#!/usr/bin/sh

CFLAGS="-Wall -Wextra"
CLIENT_SRC="src/client.c"
SERVER_SRC="src/server.c"
LIBS="-lenet"

cc -o ep-client $CFLAGS "${@:1}" $CLIENT_SRC $LIBS
cc -o ep-server $CFLAGS "${@:1}" $SERVER_SRC $LIBS
