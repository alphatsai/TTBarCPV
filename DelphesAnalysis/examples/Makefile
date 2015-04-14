
CXXFLAGS = $(shell root-config --cflags) -Wno-write-strings -D_FILE_OFFSET_BITS=64 -DDROP_CGAL -I../Delphes -I../Delphes/external -I../Delphes/external/tcl
DELPHES_LIBS = $(shell root-config --libs) -lEG -L../Delphes -lDelphes

all:
	g++ -c ana_delphes.cc $(CXXFLAGS)
	g++ -o ana_delphes ana_delphes.o $(LDFLAGS) $(DELPHES_LIBS)
