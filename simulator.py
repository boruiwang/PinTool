#!/usr/bin/python
from optparse import OptionParser
import sys, os

class PIM(object):
	"""docstring for ClassName"""
	def __init__(self, input):
		self.filename = input
		self.LogList = []

	def ParsePIMLog(self):
		with open(self.filename, 'r') as file:
			start = file.readline()
			while start != '==========Total instruction=========\n':
				start = file.readline()
			
			for i in range(6):
				self.LogList.append(file.readline().split('=')[1].strip(' '))
	
	def CalLatency(self):
		#Latencylist = [fix_comp, fix_mem, prog_comp, prog_mem, comp, mem      ]
		LatencyList = [sys.argv[3], '0', sys.argv[4], '0', sys.argv[5], sys.argv[6]]
		exe_time = 0

		for i in range(6):
			latency = float(LatencyList[i])
			instrCount = float(self.LogList[i])
			exe_time += latency * instrCount

		#                offloading_lat     + return_lat          + sync_lat  
		exe_time += 2 * (float(sys.argv[7]) + float(sys.argv[8])) + float(sys.argv[9])

		return exe_time

		
def usage():
	usage = "usage: %prog -f logfile fixPIM_comp progPIM_comp comp mem offloading return sync"
	parser = OptionParser(usage=usage)
	parser.add_option("-f", "--file", type=="string", dest="PIMLog",
					    help="parse PIM log file", metavar="FILE")

	(options, args) = parser.parse_args()
	
	# check log file
	if options.PIMLog == None:
		print "Please specify PIM Log\n"
		parser.print_help()
		sys.exit(4)
	
	# check input
	if len(sys.argv) != 10:
		print "Error: input parameters is wrong, Please check your input!"
		sys.exit(4)
	
	# check log file and then return the simulator
	if os.path.isfile(options.PIMLog):
		Simulator = PIM(options.PIMLog)
		return Simulator

	else:
		print 'Error: Unable to find PIM log file specified. Expected location: ', options.PIMLog
		sys.exit(4)

def main():
	Simulator = usage()
	
	Simulator.ParsePIMLog()
	exe_time = Simulator.CalLatency()
	print exe_time

if __name__ == "__main__":
	main()