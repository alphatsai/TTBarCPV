#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from fuctions import *
from PIDs     import PIDs  
from evtInfo  import evtInfo

class readLHE:
	def __init__(self, decayListFile, lheFile):
		self.pids =	PIDs(decayListFile)
		self.lheFileName = lheFile
		self.lheFile = []		
		self.MGVersion=''
		self.MG5ProcCard=''
		self.MGProcCard=''
		self.MGRunCard=''
		self.slha=''
		self.init=''
		self.loadedEvts=False
		self.loadedSpEvts=False
		self.spEvt=-1
		self.maxEvts=0
		self.numEvts=-1
		self.GMProcess=[]
		self.evtTable=[]
		self.events=[]
		self.tags = { 'MGVersion'  :0, 
                      'MG5ProcCard':0,
                      'MGProcCard' :0,
                      'MGRunCard'  :0,
                      'slha' :0,
                      'init' :0, 
                      'event':0, 
        } # 0: Off, 1: On, 2: Done 

	def switchTag(self, line):
		if line.find('<MGVersion')   >= 0:  self.tags['MGVersion'] = 1
		if line.find('</MGVersion')  >= 0:  self.tags['MGVersion'] =-1
		if line.find('<MG5ProcCard') >= 0:  self.tags['MG5ProcCard'] = 1
		if line.find('</MG5ProcCard')>= 0:  self.tags['MG5ProcCard'] =-1
		if line.find('<MGProcCard') >= 0:   self.tags['MGProcCard'] = 1
		if line.find('</MGProcCard')>= 0:   self.tags['MGProcCard'] =-1
		if line.find('<MGRunCard')   >= 0:  self.tags['MGRunCard'] = 1
		if line.find('</MGRunCard')  >= 0:  self.tags['MGRunCard'] =-1
		if line.find('<slha')  >= 0:        self.tags['slha'] = 1
		if line.find('</slha') >= 0:        self.tags['slha'] =-1
		if line.find('<init')  >= 0:        self.tags['init'] = 1
		if line.find('</init') >= 0:        self.tags['init'] =-1
		if line.find('<event') >= 0:        self.tags['event'] = 1
		if line.find('</event')>= 0:        self.tags['event'] =-1

	def loadLHE(self):
		print 'Loading pid informations...'
		self.pids.loadDecayList()

		print 'Loading LHE informations...'
		self.lheFile = open(self.lheFileName)

		print '>> Refomating all informations...'
		self.numEvts=-1 # Initialize, in case
		for line in self.lheFile:
			l = line.strip()
			if isEmpty(l):
				continue
			if l.find('<LesHouchesEvents') >= 0: 
				LHEVersion = l.split('="')[1].split('">')[0]	
			self.switchTag(l)
			if self.tags['MGVersion'] == 1 and l.find('<MGVersion') < 0:
				self.MGVersion = self.MGVersion + l + "\n"
			if self.tags['MG5ProcCard'] == 1 and l.find('<MG5ProcCard') < 0:
				self.MG5ProcCard = self.MG5ProcCard + l + "\n"
			if self.tags['MGProcCard'] == 1 and l.find('<MGProcCard') < 0:
				self.MGProcCard = self.MGProcCard + l + "\n"
			if self.tags['MGRunCard'] == 1 and l.find('<MGRunCard') < 0:
				self.MGRunCard = self.MGRunCard + l + "\n"
			if self.tags['slha'] == 1 and l.find('<slha') < 0:
				self.slha = self.slha + l + "\n"
			if self.tags['init'] == 1 and l.find('<init') < 0:
				self.init = self.init + l + "\n"
			if self.tags['event'] == 1 and l.find('<event') < 0:
				if isComment(l): 
					continue
				if len(l.split()) == 6:
					self.numEvts += 1	
					self.evtTable.append(l)
				else:
					self.evtTable[self.numEvts] = self.evtTable[self.numEvts] + "\n" + l 

		print '>> Getting MG process...'
		for line in self.MGProcCard.splitlines():
			if isComment(line): 
				continue
			if line.find('#Process') >= 0:
				self.GMProcess.append(line.split('#')[0].strip())

		
	def loadEvents(self, maxEvt=None ):
		if self.loadedSpEvts:
			print '|'
			print '| [Error] Do not use loadEvents() after loadSpEvents()'
			print '|'
			sys.exit()
		self.loadedEvts=True
		self.maxEvts = maxEvt or self.numEvts+1
		print 'Getting %d event informations...' % self.maxEvts
		i=0	
		while ( i < self.maxEvts ):
			self.events.append(evtInfo())
			self.events[i].fillEvtInfo(self.evtTable[i])
			i+=1

	def loadSpEvents(self, evt ):
		if self.loadedEvts:
			print '|'
			print '| [Error] Do not use loadSpEvents() after loadEvents()'
			print '|'
			sys.exit()
		self.loadedSpEvts=True
		self.spEvt = evt
		self.maxEvts = 1
		print 'Getting No.%d event informations...' % self.spEvt
		self.events.append(evtInfo())
		self.events[0].fillEvtInfo(self.evtTable[self.spEvt])

	def showEvent(self, maxEvt):
		if not self.loadedEvts:
			print '|'
			print '| [Error] Please loadEvents() before showEvent'
			print '|'
			sys.exit()
		if maxEvt <= self.maxEvts:
			i=0
			while ( i < maxEvt ):
				self.showSpEvent(i)
				i+=1
		else:
			print '|'
			print '| [Error] Out of loaded events range '+str(self.maxEvts)
			print '|'
			sys.exit()

	def showSpEvent(self, evt):
		i=None
		if not self.loadedEvts and not self.loadedSpEvts:
			print '|'
			print '| [Error] Please loadEvents() or loadSpEvents() before showSpEvent'
			print '|'
			sys.exit()
		elif self.loadedEvts and not self.loadedSpEvts: i=evt
		elif not self.loadedEvts and self.loadedSpEvts:
			if self.spEvt != evt:
				print '|'
				print '| [Error] loadSpEvents(evt1) and showSpEvent(evt2), evt1={0:d} != evt2={1:d}'.format(self.spEvt, evt)
				print '|'
				sys.exit()
			i=0

		if i < self.maxEvts:
			## * Print out event information 
			print '\n.----------------------------------------------------------------------------------.' 
			print '| *** Event %-7s **** ----------------------------------------------------------|' % str(evt)
			print '|----------------------------------------------------------------------------------|' 
			print '| Status = -1 : Incoming particle                                                  |' 
			print '|          +1 : Outgoing final state particle                                      |'
			print '|          -2 : Intermediate space-like propagator, x and Q2 shell be preserved    |' 
			print '|          +2 : Intermediate resonance, Mass should be preserved                   |' 				
			print '|          +3 : Intermediate resonance, for documentation only                     |' 				
			print '|          -9 : Incoming beam particles at time t = -oo                            |' 			
	
			for pro in self.GMProcess:
				print '| MG5 Process : %-67s|' % pro				
					
			print '| Number of particle : %2d                                                          |' % self.events[i].evtInfo_numParticle				
			print '|----------------------------------------------------------------------------------|' 
			print '| {0:20s}{1:6s} {2:7s} {3:7s} {4:8s} {5:8s} {6:6s} |'.format('Particle chain', ' | Status','|   Px  ', ' |    Py  ', '|    Pz   ', '|  Energy ', '|  Mass ')
			p=0
			while ( p < self.events[i].evtInfo_numParticle ):
				print '|----------------------------------------------------------------------------------|' 
				print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format(self.pids.showName(self.events[i].particleInfo_pid[p]), self.events[i].particleInfo_status[p], self.events[i].particleInfo_px[p], self.events[i].particleInfo_py[p], self.events[i].particleInfo_pz[p], self.events[i].particleInfo_energy[p], self.events[i].particleInfo_mass[p])
				dua1=self.events[i].particleInfo_dau1[p]
				dua2=self.events[i].particleInfo_dau2[p]
				dua3=self.events[i].particleInfo_dau3[p]
				if self.events[i].particleInfo_Ndau[p] == 3:
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('|-> '+self.pids.showName(self.events[i].particleInfo_pid[dua1]), self.events[i].particleInfo_status[dua1], self.events[i].particleInfo_px[dua1], self.events[i].particleInfo_py[dua1], self.events[i].particleInfo_pz[dua1], self.events[i].particleInfo_energy[dua1], self.events[i].particleInfo_mass[dua1])
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('|-> '+self.pids.showName(self.events[i].particleInfo_pid[dua2]), self.events[i].particleInfo_status[dua2], self.events[i].particleInfo_px[dua2], self.events[i].particleInfo_py[dua2], self.events[i].particleInfo_pz[dua2], self.events[i].particleInfo_energy[dua2], self.events[i].particleInfo_mass[dua2])
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('`-> '+self.pids.showName(self.events[i].particleInfo_pid[dua3]), self.events[i].particleInfo_status[dua3], self.events[i].particleInfo_px[dua3], self.events[i].particleInfo_py[dua3], self.events[i].particleInfo_pz[dua3], self.events[i].particleInfo_energy[dua3], self.events[i].particleInfo_mass[dua3])
				elif self.events[i].particleInfo_Ndau[p] == 2:
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('|-> '+self.pids.showName(self.events[i].particleInfo_pid[dua1]), self.events[i].particleInfo_status[dua1], self.events[i].particleInfo_px[dua1], self.events[i].particleInfo_py[dua1], self.events[i].particleInfo_pz[dua1], self.events[i].particleInfo_energy[dua1], self.events[i].particleInfo_mass[dua1])
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('`-> '+self.pids.showName(self.events[i].particleInfo_pid[dua2]), self.events[i].particleInfo_status[dua2], self.events[i].particleInfo_px[dua2], self.events[i].particleInfo_py[dua2], self.events[i].particleInfo_pz[dua2], self.events[i].particleInfo_energy[dua2], self.events[i].particleInfo_mass[dua2])
				elif self.events[i].particleInfo_Ndau[p] == 1:
					print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format('`-> '+self.pids.showName(self.events[i].particleInfo_pid[dua1]), self.events[i].particleInfo_status[dua1], self.events[i].particleInfo_px[dua1], self.events[i].particleInfo_py[dua1], self.events[i].particleInfo_pz[dua1], self.events[i].particleInfo_energy[dua1], self.events[i].particleInfo_mass[dua1])
				p+=1
			print '`----------------------------------------------------------------------------------\''
		else:
			print '|'
			print '| [Error] Out of loaded events range '+str(self.maxEvts)
			print '|'
			sys.exit()
	
