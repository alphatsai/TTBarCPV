#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from PIDs import PIDs  

print "|"
if len(sys.argv)<2:
	print "| Please give a PID"
	print "|  e.x. "+sys.argv[0]+" 12"
	print "|"
else:
	pids = PIDs("decayList.txt")
	pids.loadDecayList()	
	print "| Paritcle ID "+sys.argv[1]+" = "+pids.printParticle(sys.argv[1])
	print "|"
