#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from optparse import OptionParser

sys.path.insert(1,os.path.dirname(os.path.abspath(__file__))+'/../')
from readLHE  import readLHE 

# usage description
usage = """
 Usage: """+sys.argv[0]+""" [options]
 Example: """+sys.argv[0]+""" --max 12 --lhe unweighted_events.lhe
 For more help: """+sys.argv[0]+""" --help 
"""

# Option parameters
parser = OptionParser(usage=usage)
parser.add_option("-m", "--max", dest="maxEvt", type="int", 
                  help="Show events until maximum event")
parser.add_option("-e", "--evt", dest="evt", type="int", 
                  help="Show spacial event")
parser.add_option("-a", "--all", dest="showAll", default=False, 
                  help="Show all events or not")
parser.add_option("-d", "--data", dest="data", default='pid/decayList.txt',
                  help="Input particle decay list from pythia")
parser.add_option("-l", "--lhe", dest="lheFile", 
                  help="Input lhe file")
(options, args) = parser.parse_args()

# Main progress
if not options.lheFile:
	print usage
	sys.exit()

if not options.maxEvt and not options.evt and not options.showAll:
	print usage
	sys.exit()

if options.maxEvt and not options.evt:
	lheInfo = readLHE( options.data, options.lheFile )
	lheInfo.loadLHE()
	lheInfo.loadEvents(options.maxEvt)
	lheInfo.showEvent(options.maxEvt)	
elif not options.maxEvt and options.evt:
	lheInfo = readLHE( options.data, options.lheFile )
	lheInfo.loadLHE()
	lheInfo.loadSpEvents(options.evt) # Only load interested event 
	lheInfo.showSpEvent(options.evt)	
elif options.maxEvt and options.evt:
	print " [Error] Shell not use --max and --evt at the same time"
	print usage
	sys.exit()

if options.showAll:
	lheInfo = readLHE( options.data, options.lheFile )
	lheInfo.loadLHE()
	lheInfo.loadEvents()              # Load all events
	lheInfo.showEvent(lheInfo.maxEvts)	
	
