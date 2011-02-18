DEPS="main.o map.o entity.o"
LIBS="-L./libtcod-1.5.1 -ltcod"

redo-ifchange $DEPS

gcc $DEPS $LIBS -o $3
