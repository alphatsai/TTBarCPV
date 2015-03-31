#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from checkLineFunc import*

class PIDs:

	def __init__(self, decayListFile):
		self.decayListFile = decayListFile
		self.decayTable =[]	
		self.PIDToName = {}
		self.NameToPID = {}
		#self.NameToLower = {}

	def loadDecayList(self):
		if not os.path.isfile(self.decayListFile):
			print "|" 
			print "| [ERROR] Can't find "+self.decayListFile+", or it's not a file..."
			print "|" 
			sys.exit() 
		else:
			self.decayTable = open(self.decayListFile, 'r')
			self.mapPIDs()

	def mapPIDs(self):
		for line in self.decayTable:
			l = line.strip() 				# Remove whitespace in begion and end of line
			if isComment(l) or isEmpty(l):  # Remove comment and empty line
				continue
			ls = l.split()
			if len(ls) > 10: 
				antiPID=-1*int(ls[0])
				self.PIDToName[int(ls[0])]=ls[2]
				self.PIDToName[antiPID]=ls[3]
				self.NameToPID[ls[2]]=int(ls[0])
				self.NameToPID[ls[3]]=antiPID
				#print ls[0]+" -> "+ls[2] #DEBUG
		#print self.PIDToName #DEBUG

	def showPID(self, name):
		if not str(name) in self.NameToPID:
			return 'Undefined Name' 
		else: 
			return self.NameToPID[str(name)]


	def showName(self, pid):
		if not int(pid) in self.PIDToName:
			return 'Undefined PID' 
		else: 
			return self.PIDToName[int(pid)]
