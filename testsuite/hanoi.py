def hanoi(ndisks, startPeg, endPeg):
    if ndisks > 0:
		a = ndisks-1
		c = 6-startPeg-endPeg
        hanoi(a, startPeg, c)
        # print "Move disk %d from peg %d to peg %d" % (ndisks, startPeg, endPeg)
		print ndisks
		print startPeg
		print endPeg
        hanoi(a, c, endPeg)
	end
end

def main():
	hanoi(4, 1, 3)
end