
CXXFLAGS = $(shell root-config --cflags) -Wno-write-strings -D_FILE_OFFSET_BITS=64 -DDROP_CGAL -IDelphes -IDelphes/external -IDelphes/external/tcl
DELPHES_LIBS = $(shell root-config --libs) -lEG -LDelphes -lDelphes

all:
	g++ -c analysis.cc $(CXXFLAGS)
	g++ -o analysis analysis.o $(LDFLAGS) $(DELPHES_LIBS)
