redo-ifchange $1.c

CFLAGS="-std=gnu99 -O3"
INCLUDE="-I./libtcod-1.5.1/include"

gcc $CFLAGS $INCLUDE -MD -MF $1.d -c -o $3 $1.c

read DEPS <$1.d
redo-ifchange ${DEPS#*:}
rm $1.d
