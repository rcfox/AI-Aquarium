for file in `find . -name 'libtcod*' -prune -o -name '*.c' -print`; do
	DEPS="$DEPS ${file%.c}.o ";
done

LIBS="-L./libtcod-1.5.1 -ltcod"
CFLAGS=""

BASEDIR=`pwd` redo-ifchange $DEPS

gcc $DEPS $LIBS -o $3
