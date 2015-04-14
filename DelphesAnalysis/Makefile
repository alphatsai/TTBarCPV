
CXXFLAGS = $(shell root-config --cflags) -Wno-write-strings -D_FILE_OFFSET_BITS=64 -DDROP_CGAL -IDelphes -IDelphes/external -IDelphes/external/tcl
DELPHES_LIBS = $(shell root-config --libs) -lEG -LDelphes -lDelphes

all:
	g++ -c src/analysis.cc $(CXXFLAGS)
	g++ -o src/analysis analysis.o $(LDFLAGS) $(DELPHES_LIBS)
	rm -f analysis.o
