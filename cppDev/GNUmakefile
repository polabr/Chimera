#CC=clang++
CC=g++

INCLUDES=`root-config --cflags`
INCLUDES+=`larlite-config --includes`
INCLUDES+=`larcv-config --includes`

LIBS=`root-config --glibs`
LIBS+=`larlite-config --libs`
LIBS+=`larcv-config --libs`
# Note: for larlite/larcv libs to work, go into dlee_unified and do source setup.sh, configure.sh

all: find1mu1p put2TracksTogether

find1mu1p: find1mu1p.cxx findMuon.cxx findProton.cxx 
	$(CC) -Wall -o find1mu1p find1mu1p.cxx findMuon.cxx findProton.cxx $(INCLUDES) $(LIBS)

put2TracksTogether: put2TracksTogether.cxx
	$(CC) -Wall -o put2TracksTogether put2TracksTogether.cxx -g $(INCLUDES) $(LIBS)
