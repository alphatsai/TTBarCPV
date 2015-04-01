#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from fuctions import*
from PIDs import PIDs  

### 1) class evtInfo
class evtInfo: 
	def __init__(self):
		self.evtInfo_numParticle = ''	
		self.evtInfo_idProcess   = ''	
		self.evtInfo_evtWrt      = ''	
		self.evtInfo_scalePDF    = ''	
		self.evtInfo_aQED    	 = ''	
		self.evtInfo_aQCD    	 = ''	
		self.particleInfo_pid  	 = []
		self.particleInfo_status = []
		self.particleInfo_moth1  = []
		self.particleInfo_moth2  = []
		self.particleInfo_Nmoth  = []
		self.particleInfo_dau1   = []
		self.particleInfo_dau2   = []
		self.particleInfo_dau3   = []               # Sometimes heppand
		self.particleInfo_Ndau   = []
		self.particleInfo_color1 = []
		self.particleInfo_color2 = []
		self.particleInfo_px     = []
		self.particleInfo_py     = []
		self.particleInfo_pz     = []
		self.particleInfo_energy = []
		self.particleInfo_mass   = []
		self.particleInfo_invLifeTimie = []         # without secondary vertex, which depend on generating setup
		self.particleInfo_helicity     = []

	def fillEvtInfo(self, evtTable):
		p=0
		for row in evtTable.splitlines():
			## * Fist line of event table, which contain event information
			if len( row.split() ) == 6:  
				self.evtInfo_numParticle = int(row.split()[0])
				self.evtInfo_idProcess   = int(row.split()[1])
				self.evtInfo_evtWrt      = float(row.split()[2])	
				self.evtInfo_scalePDF    = float(row.split()[3])	
				self.evtInfo_aQED    	= float(row.split()[4])	
				self.evtInfo_aQCD    	= float(row.split()[5])
				k=0
				while ( k < self.evtInfo_numParticle):
					self.particleInfo_Nmoth.append(0)
					self.particleInfo_Ndau .append(0)
					self.particleInfo_dau1 .append(-1)
					self.particleInfo_dau2 .append(-1)
					self.particleInfo_dau3 .append(-1)
					k+=1	
			## * Particle information 
			elif len( row.split() ) == 13:                   
				self.particleInfo_pid   .append(int(row.split()[0]))
				self.particleInfo_status.append(int(row.split()[1]))
				self.particleInfo_moth1 .append(int(row.split()[2])-1)
				self.particleInfo_moth2 .append(int(row.split()[3])-1)
				self.particleInfo_color1.append(int(row.split()[4]))
				self.particleInfo_color2.append(int(row.split()[5]))
				self.particleInfo_px    .append(float(row.split()[6]))
				self.particleInfo_py    .append(float(row.split()[7]))
				self.particleInfo_pz    .append(float(row.split()[8]))
				self.particleInfo_energy.append(float(row.split()[9]))
				self.particleInfo_mass  .append(float(row.split()[10]))
				self.particleInfo_invLifeTimie.append(float(row.split()[11]))
				self.particleInfo_helicity    .append(float(row.split()[12]))
				## * Count daughters
				if self.particleInfo_moth1[p] == self.particleInfo_moth2[p] and self.particleInfo_moth1[p] != -1:
				   self.particleInfo_Nmoth[p] = 1
				   self.particleInfo_Ndau[self.particleInfo_moth1[p]] += 1
				elif self.particleInfo_moth1[p] != self.particleInfo_moth2[p] and self.particleInfo_moth1[p] != -1 and self.particleInfo_moth2[p] != -1:
				     self.particleInfo_Nmoth[p] = 2
				     self.particleInfo_Ndau[self.particleInfo_moth1[p]] += 1
				     self.particleInfo_Ndau[self.particleInfo_moth2[p]] += 1
				p+=1	

		## * Store daughters
		p=0
		while ( p < self.evtInfo_numParticle ):
			moth1 = self.particleInfo_moth1[p]
			moth1 = self.particleInfo_moth2[p]
			### Mother 1
			if self.particleInfo_dau1[moth1] == -1 and self.particleInfo_Ndau[moth1] > 0:
			   self.particleInfo_dau1[moth1] = p
			elif self.particleInfo_dau1[moth1] > -1 and self.particleInfo_dau2[moth1] == -1:
				 self.particleInfo_dau2[moth1] = p
			elif self.particleInfo_dau1[moth1] > -1 and self.particleInfo_dau2[moth1] > -1 and self.particleInfo_dau3[moth1] == -1:
				 self.particleInfo_dau3[moth1] = p
			### Mother 2
			if self.particleInfo_dau1[moth2] == -1 and self.particleInfo_Ndau[moth2] > 0:
			   self.particleInfo_dau1[moth2] = p
			elif self.particleInfo_dau1[moth2] > -1 and self.particleInfo_dau2[moth2] == -1 and self.particleInfo_dau1[moth2] != p:
			     self.particleInfo_dau2[moth2] = p
			elif self.particleInfo_dau1[moth2] > -1 and self.particleInfo_dau2[moth2] > -1 and self.particleInfo_dau3[moth2] == -1 and self.particleInfo_dau3[moth2] != p:
			     self.particleInfo_dau3[moth2] = p
			p+=1


### 2) class evtInfo
class readLHE:
	def __init__(self, decayListFile, lheFile):
		self. =	PIDs(decayListFile)
		self.lheFileName = lheFile
		self.lheFile = []		
		self.MGVersion=''
		self.MG5ProcCard=''
		self.MGProcCard=''
		self.MGRunCard=''
		self.slha=''
		self.init=''
		self.numEvts=-1
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
			switchTag(l)
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

		
	def loadEvents(self):
		print 'Getting event informations'
		i=0	
		while ( i <= self.numEvts ):
			self.events.append(evtInfo())
			self.events[i].fillEvtInfo(self.evtTable[i])

	def showEvent(self, maxEvt):
		if maxEvt <= self.numEvts:
			i=0
			while ( i <= maxEvt ):
				self.showSpEvent(i)
				i+=1
		else:
			print '|'
			print '| [Error] Out of total events '+str(self.numEvts)
			print '|'

	def showSpEvent(self, evt):
		if i <= self.numEvts:
			## * Print out event information 
			print '\n.----------------------------------------------------------------------------------.' 
			print '| *** Event '+str(i)+' **** ----------------------------------------------------------------|'
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
				print '| {0:20s} | {1:+4d}   | {2:7.2f} | {3:7.2f} | {4:8.2f} | {5:8.2f} | {6:6.2f} |'.format(self.pids.showName(self.events[i].particleInfo_pid[p]), self.events[i].particleInfo_status[p], particleInfo_px[p], self.events[i].particleInfo_py[p], self.events[i].particleInfo_pz[p], self.events[i].particleInfo_energy[p], self.events[i].particleInfo_mass[p])
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
			print '| [Error] Out of total events '+str(self.numEvts)
			print '|'
	
