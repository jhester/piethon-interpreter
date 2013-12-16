'''This program tests recursion by calculating the fibbonacci sequence!
	This has a prerequisite of testing return values.'''	

def fibonnaci(n):
	if n == 0:
		return 0
	else:
		if n == 1:
			return 1
		else:
			n1 = n-1
			n2 = n-2
			f1 = fibonnaci(n1)
			f2 = fibonnaci(n2)
			return  f1 + f2
		end
	end	
end

def main():
	a = fibonnaci(0)
	print a
	a = fibonnaci(1)
	print a
	a = fibonnaci(2)
	print a
	a = fibonnaci(3)
	print a
	a = fibonnaci(4)
	print a
	a = fibonnaci(5)
	print a
end