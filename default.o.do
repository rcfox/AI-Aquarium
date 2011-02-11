redo-ifchange $1.c

CFLAGS="-std=gnu99 -O2"
INCLUDES="-I./libtcod-1.5.1/include"

gcc $CFLAGS $INCLUDES -MD -MF $3.deps.tmp -c -o $3 $1.c

# Checks implicit dependencies
DEPS=$(sed -e "s/^$3://" -e 's/\\//g' <$3.deps.tmp)
rm -f $3.deps.tmp
redo-ifchange $DEPS
