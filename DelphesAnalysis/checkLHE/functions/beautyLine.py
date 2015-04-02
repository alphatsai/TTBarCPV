#!/usr/bin/env python
import os, re, sys, shutil
import math, ROOT

def isComment(line):
	l = line.strip() # Remove whitespace in begion and end of line
	return l.startswith('#')

def isEmpty(line):
	l = line.strip() # Remove whitespace in begion and end of line
	if l:
		return False
	else:
		return True

