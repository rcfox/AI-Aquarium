cd t
sh make-tests.sh > all_tests.c
cd ..

for file in `find . -name 'libtcod*' -prune -o -name 'main.c' -prune -o -name '*.c' -print`; do
	DEPS="$DEPS ${file%.c}.o ";
done

LIBS="-L./libtcod-1.5.1 -ltcod -Wl,-rpath,./libtcod-1.5.1"
CFLAGS=""

BASEDIR=`pwd` redo-ifchange $DEPS

gcc $DEPS $LIBS -o $3

./$3 1>&2
