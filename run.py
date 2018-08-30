#!/usr/bin/python
import sys
import os
import time
import random
import math
pi = 3.141592654
emass = 0.5109989
jobs = 0
pwd = os.getcwd()
first = int(sys.argv[1])
last = int(sys.argv[2])
for run in range(first,last):
	cmd = '\n'
	for i in range(0,100):
		cmd = cmd + pwd+'/fit '+str(10000+run*100+i)+'\n'
	cmdfilename = "condor/sim_"+str(run)+".sh"
	cmdfile = open(cmdfilename,"w")
	cmdfile.write("#!/bin/bash\n")
	#cmdfile.write("source ~/junoenv17\n")
	cmdfile.write(cmd)
	cmdfile.close()
	os.chmod(cmdfilename,0775)
	
	condorfilename = "condor/condor_"+str(run)+".condor"
	condorfile = open(condorfilename,"w")
	condorfile.write("Executable = "+str(cmdfilename)+"\n")
	condorfile.write("Universe = vanilla\n")
	condorfile.write("getenv = TRUE\n")
	condorfile.write("should_transfer_files = YES\n")
	condorfile.write("when_to_transfer_output = ON_EXIT\n")
	condorfile.write("transfer_input_files = \n")
	condorfile.write("Log = logs/"+str(run)+".log\n")
	condorfile.write("error = logs/"+str(run)+".error\n")
	condorfile.write("output = logs/"+str(run)+".output\n")
	condorfile.write("Arguments = \n")
	condorfile.write("Queue\n")
	condorfile.close()
	
	cmd  = "condor_submit "+str(condorfilename)	
	os.system(cmd)
