def f(x):
  if x < 10:
    print x
  end
  if x == 10: print x
  end
end

def g(x, y):
  if x == y:
    print y
  end
  if x >= y:
    print y
  end
  if x != y:
    print y
  end
end

def main():
  g(2,3)
  f(9)
end
