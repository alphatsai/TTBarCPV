#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT

#print os.getcwd()
sys.path.insert(1,os.path.dirname(os.path.abspath(__file__))+'/../')
from functions import beautyLine  

class PIDs:
	def __init__(self, decayListFile):
		self.decayListFile = decayListFile
		self.decayTable =[]	
		self.PIDToName = {}
		self.NameToPID = {}
		self.LowerToName = {}                                     # For ignoring lowercase or uppercase

	def loadDecayList(self):
		if not os.path.isfile(self.decayListFile):
			print "|" 
			print "| [ERROR] Can't find "+self.decayListFile+", or it's not a file..."
			print "|" 
			sys.exit() 
		else:
			print '>> Loading %s...' %self.decayListFile
			self.decayTable = open(self.decayListFile, 'r')
			self.mapPIDs()

	def mapPIDs(self):
		for line in self.decayTable:
			l = line.strip()                                      # Remove whitespace in begion and end of line
			if beautyLine.isComment(l) or beautyLine.isEmpty(l):  # Remove comment and empty line
				continue
			ls = l.split()
			if len(ls) > 10: 
				antiPID=-1*int(ls[0])
				self.PIDToName[int(ls[0])]=ls[2]
				self.PIDToName[antiPID]=ls[3]
				self.NameToPID[ls[2]]=int(ls[0])
				self.NameToPID[ls[3]]=antiPID
				self.LowerToName[ls[2].lower()]=ls[2]
				self.LowerToName[ls[3].lower()]=ls[3]

	def showPID(self, name):
		if not self.correctName(name) in self.NameToPID:
			return 'Error '+self.correctName(name) 
		else: 
			return self.NameToPID[self.correctName(name)]

	def showName(self, pid):
		if not int(pid) in self.PIDToName:
			return 'Undefined PID - '+str(pid) 
		else: 
			return self.PIDToName[int(pid)]

	def correctName(self, name):
		if not str(name).lower() in self.LowerToName:
			return 'Undefined Name - '+str(name) 
		else: 
			return self.LowerToName[str(name).lower()]
		
