def fun(x, y):
  x = x+12
  y = x+5*y-x/2
  z = -x/y
  q = -x - y%2 -9
  p = -9
  print -x-x-z*-x 
  print y**2 
  print z 
  print p 
  print q
end

def gun(x):
  print x
end

def run(x, y):
  print x
  print y
  x = -99
  y = 0
  fun(x, y)
end

def ton(x, y):
  x = 1.5
  y = -x+1
  fun(x, y)
end

def main():
  ton(2,3)
  run(4,5)
  gun(6)
end
