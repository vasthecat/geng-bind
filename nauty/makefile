CC=gcc
CFLAGS= -O3
W=-DWORDSIZE=32
W1=-DMAXN=WORDSIZE -DWORDSIZE=32
CCOBJ=${CC} -c ${CFLAGS} -o $@
GTOOLSH=gtools.h nauty.h naututil.h nausparse.h naurng.h gutils.h \
  naugroup.h nautinv.h schreier.h nautycliquer.h traces.h \
  naugstrings.h planarity.h quarticirred28.h
NAUTYW1O=nautyW1.o nautilW1.o nausparseW.o naugraphW1.o schreierW.o naurng.o

all: libnauty.a

nautyW1.o: nauty.h schreier.h nauty.c
	${CCOBJ} ${W1} nauty.c
nautilW1.o: nauty.h nautil.c sorttemplates.c
	${CCOBJ} ${W1} nautil.c
nausparseW.o: nauty.h nausparse.h nausparse.c sorttemplates.c
	${CCOBJ} ${W} nausparse.c
naugraphW1.o: nauty.h naugraph.c
	${CCOBJ} ${W1} naugraph.c
schreierW.o : nauty.h naurng.h schreier.h schreier.c
	${CCOBJ} ${W} schreier.c
naurng.o: naurng.c nauty.h
	${CCOBJ} naurng.c
traces.o : nauty.h naurng.h schreier.h traces.h nausparse.h traces.c
	${CCOBJ} traces.c
gtoolsW.o : ${GTOOWSH} gtools.c
	${CCOBJ} ${W} gtools.c
nautinvW1.o: nauty.h naututil.h nautinv.c
	${CCOBJ} ${W1} nautinv.c
gutil1W1.o : ${GTOOWSH} gutils.h gutil1.c
	${CCOBJ} ${W1} gutil1.c
gutil2W1.o : ${GTOOWSH} gutils.h gutil2.c
	${CCOBJ} ${W1} gutil2.c
gtnautyW1.o : ${GTOOWSH} gtnauty.c sorttemplates.c
	${CCOBJ} ${W1} gtnauty.c
naugroupW.o : nauty.h naugroup.h naugroup.c
	${CCOBJ} ${W} naugroup.c
nautycliquerW.o : nauty.h naugroup.h naugroup.c nautycliquer.c nautycliquer.h
	${CCOBJ} ${W} nautycliquer.c
naututilW1.o: nauty.h naututil.h nausparse.h naututil.c
	${CCOBJ} ${W1} naututil.c

libnauty.a: ${NAUTYW1O} traces.o gtoolsW.o naututilW1.o nautinvW1.o \
	   gutil1W1.o gutil2W1.o gtnautyW1.o naugroupW.o nautycliquerW.o
	rm -f libnauty.a
	${AR} crs libnauty.a ${NAUTYW1O} traces.o gtoolsW.o naututilW1.o \
	   nautinvW1.o gutil1W1.o gutil2W1.o gtnautyW1.o naugroupW.o nautycliquerW.o

clean:
	rm -f libnauty.a ${NAUTYW1O} traces.o gtoolsW.o naututilW1.o \
	      nautinvW1.o gutil1W1.o gutil2W1.o gtnautyW1.o \
	      naugroupW.o nautycliquerW.o
