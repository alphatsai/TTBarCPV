#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from checkLineFunc import*

class PIDs:

	def __init__(self, decayListFile):
		self.decayListFile = decayListFile
		self.decayTable =[]	
		self.PIDs = {}

	def loadDecayList(self):
		if not os.path.isfile(self.decayListFile):
			print "[ERROR] Can't find "+decayListi+", or it's not a file..."
			return 
		else:
			self.decayTable = open(self.decayListFile, 'r')
			self.mapPIDs()

	def mapPIDs(self):
		for line in self.decayTable:
			l = line.strip() 				# Remove whitespace in begion and end of line
			if isComment(l) or isEmpty(l):  # Remove comment and empty line
				continue
			ls = l.split()
			if len(ls) == 12: 
				self.PIDs[int(ls[0])]=ls[2]
				antiPID=-1*int(ls[0])
				self.PIDs[antiPID]=ls[3]
		#print self.PIDs #DEBUG

	def printParticle(self, pid):
		return self.PIDs[int(pid)]


