#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

from fuctions import *
from PIDs     import PIDs  

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



