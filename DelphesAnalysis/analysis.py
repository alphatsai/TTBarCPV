#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT

from optparse import OptionParser

# usage description
usage = """
 Usage: """+sys.argv[0]+""" [options]
 Example: """+sys.argv[0]+""" --write results --input delphes.root
 For more help: """+sys.argv[0]+""" --help 
"""

# Option parameters
parser = OptionParser(usage=usage)
parser.add_option("-w", "--write", dest="writeInto", default='results',
                  help="Write into folder")
parser.add_option("-c", "--code", dest="anaCode", default='./src/analysis',
                  help="Analysis code")
parser.add_option("-i", "--input", dest="inPut", 
                  help="Input delphes root file")
parser.add_option("-o", "--output", dest="outPut", default='result.root',
                  help="Input particle decay list from pythia")
(options, args) = parser.parse_args()

if not os.path.isfile(options.anaCode):
	print ">> [ERROR] Can't find "+options.anaCode+", or it's not a file..."
	sys.exit()
if not options.inPut:
	print usage
	sys.exit()
if options.writeInto and not os.path.isdir(options.writeInto):
	os.system('mkdir -p '+options.writeInto)	
	print '>> Created new directory: '+options.writeInto

print '>> Writing into '+options.writeInto+'...'
commands=options.anaCode+' '+options.inPut+' '+options.writeInto+'/'+options.outPut 
os.system(commands)	
