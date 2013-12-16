'''This program tests calling functions without arguments. 
	First test case that uses execution stack and stack frames.
	This is to check that we have everything scoped locally correctly.'''	
def g():
	x = 30
	print x
end

def f():
	x = 20
	print 5
	g()
	print x
end

def main():
	x = 10
	f()
	print x
end