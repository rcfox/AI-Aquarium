DEPS="main.o"
LIBS=""

redo-ifchange $DEPS

gcc $DEPS $LIBS -o $3
