#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

#if sys.argv[1] == '':
#	print ""
#	print sys.argv[0]

from fuctions import*
from PIDs import PIDs  

pids = PIDs('decayList.txt')
pids.loadDecayList()

lheFile = open('example_TTtoDiMuon_UnWrtEvents.lhe');
LHEVersion=''
MGVersion=''
MG5ProcCard=''
MGRunCard=''
slha=''
init=''
evtTable=[]
tags = { 'MGVersion':0, 
         'MG5ProcCard':0,
         'MGRunCard':0,
         'slha':0,
         'init':0, 
         'event':0, 
} # 0: Off, 1: On, 2: Done 

def switchTag(line, tags):
	if l.find('<MGVersion')   >= 0:  tags['MGVersion'] = 1
	if l.find('</MGVersion')  >= 0:  tags['MGVersion'] =-1
	if l.find('<MG5ProcCard') >= 0:  tags['MG5ProcCard'] = 1
	if l.find('</MG5ProcCard')>= 0:  tags['MG5ProcCard'] =-1
	if l.find('<MGRunCard')   >= 0:  tags['MGRunCard'] = 1
	if l.find('</MGRunCard')  >= 0:  tags['MGRunCard'] =-1
	if l.find('<slha')  >= 0:        tags['slha'] = 1
	if l.find('</slha') >= 0:        tags['slha'] =-1
	if l.find('<init')  >= 0:        tags['init'] = 1
	if l.find('</init') >= 0:        tags['init'] =-1
	if l.find('<event') >= 0:        tags['event'] = 1
	if l.find('</event')>= 0:        tags['event'] =-1

numEvt=-1
for line in lheFile:

	l = line.strip()
	if isEmpty(l):
		continue

	if l.find('<LesHouchesEvents') >= 0: 
		LHEVersion = l.split('="')[1].split('">')[0]	
		 
	switchTag(l, tags)
	if tags['MGVersion'] == 1 and l.find('<MGVersion') < 0:
		MGVersion = MGVersion + l + "\n"
	if tags['MG5ProcCard'] == 1 and l.find('<MG5ProcCard') < 0:
		MG5ProcCard = MG5ProcCard + l + "\n"
	if tags['MGRunCard'] == 1 and l.find('<MGRunCard') < 0:
		MGRunCard = MGRunCard + l + "\n"
	if tags['slha'] == 1 and l.find('<slha') < 0:
		slha = slha + l + "\n"
	if tags['init'] == 1 and l.find('<init') < 0:
		init = init + l + "\n"
	if tags['event'] == 1 and l.find('<event') < 0:
		if isComment(l): 
			continue
		if len(l.split()) == 6:
			numEvt += 1	
			evtTable.append(l)
		else:
			evtTable[numEvt] = evtTable[numEvt] + "\n" + l 

i=0
while ( i <= numEvt ):
		
		#print '|' 
		print '| Event '+str(i)+' --------------------------'

		evtInfo_numParticle = ''	
		evtInfo_idProcess   = ''	
		evtInfo_evtWrt      = ''	
		evtInfo_scalePDF    = ''	
		evtInfo_aQED    	= ''	
		evtInfo_aQCD    	= ''	
		particleInfo_pid   	= []
		particleInfo_status = []
		particleInfo_moth1  = []
		particleInfo_moth2  = []
		particleInfo_color1 = []
		particleInfo_color2 = []
		particleInfo_px     = []
		particleInfo_py     = []
		particleInfo_pz     = []
		particleInfo_energy = []
		particleInfo_mass   = []
		particleInfo_invLifeTimie = [] # without secondary vertex, which depend on generating setup
		particleInfo_helicity = []
		for row in evtTable[i].splitlines():
			if len( row.split() ) == 6:
				print row
				evtInfo_numParticle = row.split()[0]
				evtInfo_idProcess   = row.split()[1]
				evtInfo_evtWrt      = row.split()[2]	
				evtInfo_scalePDF    = row.split()[3]	
				evtInfo_aQED    	= row.split()[4]	
				evtInfo_aQCD    	= row.split()[5]	
			elif len( row.split() ) == 13:
				particleInfo_pid.append(row.split()[0])
				particleInfo_status.append(row.split()[1])
				particleInfo_moth1.append(row.split()[2])
				particleInfo_moth2.append(row.split()[3])
				particleInfo_color1.append(row.split()[4])
				particleInfo_color2.append(row.split()[5])
				particleInfo_px.append(row.split()[6])
				particleInfo_py.append(row.split()[7])
				particleInfo_pz.append(row.split()[8])
				particleInfo_energy.append(row.split()[9])
				particleInfo_mass.append(row.split()[10])
				particleInfo_invLifeTimie.append(row.split()[11])
				particleInfo_helicity.append(row.split()[12])
				#pid      = row.split()[0]
				#status   = row.split()[1]
				#moth1    = row.split()[2]
				#moth2    = row.split()[2]
				#px       = row.split()[6]
				#py       = row.split()[7]
				#pz       = row.split()[8]
				#energy   = row.split()[9]
				#mass     = row.split()[10]
				#helicity = row.split()[12]


			print particleInfo_pid
			#print '| '+pids.showName(pid)
			#print '|' 

		i+=1	


#print MGVersion
#print MG5ProcCard
#print MGRunCard
#print slha 
#print init
#print event



#import xml.etree.ElementTree as lheFile 
#lheTree = lheFile.parse('example_TTtoDiMuon_UnWrtEvents.lhe')
#lheRoot = lheTree.getroot()
#
#for event in lheRoot.findall('event'):
#	evtProcess=''
#	for contain in event.text:				# Merge contains be a line
#		evtProcess = evtProcess + contain
#	for line in evtProcess:
#		print line
#	#print evtProcess 
#
#for leaf in lheRoot:
#	print leaf.tag	
