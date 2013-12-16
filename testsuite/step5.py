'''This program tests calling functions WITH arguments / parameters.
	Also tests whether we check to see if number of arguments is correct'''	
def h(a,b,c,d,e,f,g):
	print a
	print b
	print c
	print d
	print e
	print f
	print g
end

def g(z):
	print z
end
def f(x, z):
	y = x / 2
	print x
	print y
	print z
	g(z)
	x = x - 1
	g(x)
end
		
def main():
	x = 10
	f(14,x)
	print x
	h(1,2,3,4,5,6,x)
end