#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

#if sys.argv[1] == '':
#	print ""
#	print sys.argv[0]

from fuctions import*

lheFile = open('example_TTtoDiMuon_UnWrtEvents.lhe');
LHEVersion=''
MGVersion=''
MG5ProcCard=''
MGRunCard=''
slha=''
init=''
event=''
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
		event = event + l + "\n"


#print MGVersion
#print MG5ProcCard
#print MGRunCard
#print slha 
#print init
#print event

#	if not isComment(l) and not isEmpty(l):
#		if l.find('<LesHouchesEvents') >= 0:
#			version = l.split('="')[1].split('">')[0]	
#		if l.find('<LesHouchesEvents') >= 0:
#			print version	










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
