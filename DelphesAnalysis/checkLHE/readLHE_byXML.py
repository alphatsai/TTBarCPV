#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

#if sys.argv[1] == '':
#	print ""
#	print sys.argv[0]

from fuctions import*
from PIDs import PIDs

import xml.etree.ElementTree as lheFile 
lheTree = lheFile.parse('example_TTtoDiMuon_UnWrtEvents.lhe')
lheRoot = lheTree.getroot()

for event in lheRoot.findall('event'):
	evtProcess=''
	for contain in event.text.splitlines():
		if isEmpty(contain) or isComment(contain):
			continue
		print contain

#for leaf in lheRoot:
#	print leaf.tag
