#!/usr/bin/python
from optparse import OptionParser
import sys, os

class PIM(object):
	"""docstring for ClassName"""
	def __init__(self, input):
		self.filename = input
		self.LogList = []	# LogList stores PIM output

	def ParsePIMLog(self):
		with open(self.filename, 'r') as file:
			start = file.readline()
			while start != '==========Total instruction=========\n':
				start = file.readline()
			
			for i in range(6):
				self.LogList.append(file.readline().split('=')[1].strip(' '))
	
	def CalLatency(self):
		#Latencylist = [fix_comp, fix_mem, prog_comp, prog_mem, comp, mem      ]
		fix_mem = '0'
		prog_mem = '0'
		LatencyList = [sys.argv[3], fix_mem, sys.argv[4], prog_mem, sys.argv[5], sys.argv[6]]
		exe_time = 0

		# comp + mem
		for i in range(6):
			latency = float(LatencyList[i])
			instrCount = float(self.LogList[i])
			exe_time += latency * instrCount

		# offloading
		for i in range(7, 11):
			exe_time += float(sys.argv[i])
 		
 		# sync
		exe_time += float(sys.argv[11])

		return exe_time

		
def usage():
	usage = "usage: %prog -f logfile fixPIM_comp progPIM_comp comp mem\
									 fixPIM_offloading fixPIM_return progPIM_offloading progPIM_return sync"
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
	if len(sys.argv) != 12:
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