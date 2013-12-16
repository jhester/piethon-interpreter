'''Testing out the use of arrays
	Obviously limited, since we dont have classes.
	We can only create, assign by index, fetch by index and use a limited number of operations'''
def test(array):
	array.append(11)
	return array
end

def clear(array):
	while len(array) > 0:
		array.pop()
	end
	return array
end

def main():
	b = 9
	a = [1,b,3.2,4,5.2,6,7,8]
	d = []
	print a
	print a[0]
	c = a[4]
	print c
	a[5] = 22
	print a[5]
	print a
	# empty list test
	print d
	# length of a list test
	e = len(a)
	print e
	# this throws an error len(b)
	
	d.append(2)
	d.append(6)
	print d
	d.pop()
	print d
	d = test(d)
	print d
	a = clear(a)
	print a
end