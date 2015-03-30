#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT
import numpy

RunNo = 0
LumiNo = 1
EvtNo = 2

outfile = open('SameEventsWithBprimeTobH.txt', 'w')

bHFile = 'BprimeTobH_Events'
allFile = ['B2G_12_019_data', 'BprimeTobZ_Events',  'MultileptonEventList',  'SS2L_Event_lists']
#allFile = ['SS2L_Event_lists']
f_bH = open(str(bHFile+'.txt'), 'r')
lines_bH = filter(None, (line.rstrip() for line in f_bH)) #Ingore empty line
print '[RunNo, LumiNo, EvtNo]'
outfile.write('RunNo\tLumiNo\tEvtNo') 
for fileName in allFile:
	print 'Looking for same events in '+str(fileName+".txt")+' with '+str(bHFile+'.txt')+'...'
	outfile.write('\n') 
	outfile.write('Comparing with '+str(fileName+".txt")+'\n') 
	#result = numpy.loadtxt(open(str(fileName+".txt"),"r"),delimiter=" ",skiprows=1).astype('int')
	#result.sort()
	#print result 
	with open(str(fileName+".txt")) as f_in:
		lines = filter(None, (line.rstrip() for line in f_in)) #Ingore empty line
		for line in lines:
			if ("Lumi" or "No" or "no" or "num") in line: continue 
			BpToOtherEvt = line.split() # [RunNo, LumiNo, EvtNo]
			#BpToOtherEvt = map(int, BpToOtherEvt) #string to int
			#BpToOtherEvt.sort() #For fun
			for line_bH in lines_bH: 	
				BpTobHEvt = line_bH.split()
				#BpTobHEvt = map(int, BpTobHEvt)
				if ((  BpToOtherEvt[RunNo] == BpTobHEvt[RunNo] ) 
				and ( BpToOtherEvt[LumiNo] == BpTobHEvt[LumiNo])
				and ( BpToOtherEvt[EvtNo]  == BpTobHEvt[EvtNo] )):  
					print '[ '+BpTobHEvt[RunNo]+' '+BpTobHEvt[LumiNo]+' '+BpTobHEvt[EvtNo]+' ]'	
					outfile.write(BpTobHEvt[RunNo]+'\t'+BpTobHEvt[LumiNo]+'\t'+BpTobHEvt[EvtNo]+'\n')
