#!/usr/bin/python

import os
import sys
import subprocess

if __name__ == "__main__":
	print("Begin test\n")
	print("$ echo 'set 1 13' > /proc/{0}/fd/0".format(6904) )
	pid = subprocess.call('(pidof rdb) ' , shell=True )
	print("rdb pid: {0}\n".format( pid ) )
	
	
