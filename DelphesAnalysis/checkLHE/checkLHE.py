#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

if sys.argv[1] == '':
	print ""
	print sys.argv[0]

##### * My function * #####
def isComment(line):
	l = line.strip() # Remove whitespace in begion and end of line
	return l.startswith('#')

def isEmpty(line):
	l = line.strip() # Remove whitespace in begion and end of line
	if l:
		return False
	else:
		return True
	
##### * Main Class * #####
decayTable = open('decayList.txt', 'r')
#decayTable = open('test.txt', 'r')
PIDs={}
for line in decayTable:
	l = line.strip() 				# Remove whitespace in begion and end of line
	if isComment(l) or isEmpty(l):  # Remove comment and empty line
		continue
	ls = l.split()
	if len(ls)  == 12: 
		PIDs[int(ls[0])]=ls[2]
		antiPID=-1*int(ls[0])
		PIDs[antiPID]=ls[3]

def 
