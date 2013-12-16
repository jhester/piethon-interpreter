def f(x):
  if x < 10:
    print x
  end
end

def g(x, y):
  if x == y:
    print x
  else:
    y = x
    print y
  end
end

def f(x):
  if x < 10:
    print x
  else: x=2*x**2
  end
  if x == 10: print x
  end
end

def main(x, y):
  if x == y:
    print y
  else:
    x = x**y+1
  end
  if x >= y:
    print y
  end
  if x != y:
    print y
  end
end
