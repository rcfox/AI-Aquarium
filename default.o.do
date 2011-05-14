redo-ifchange $1.c

CFLAGS="-std=gnu99 -ggdb -Wall"
INCLUDE="-I$BASEDIR/libtcod-1.5.1/include -I$BASEDIR"

gcc $CFLAGS $INCLUDE -MD -MF $1.d -c -o $3 $1.c

read DEPS <$1.d
redo-ifchange ${DEPS#*:}
rm $1.d
