def f(x):
  if x < 10:
    x = x+1
    print x
    f(x)
  end
  return 7
end

def g(x, y):
  if x == y:
    f(x)
  end
  return y
end

def h():
  x = 1
  y = 1
  g(x, y)
  return x
end

def main():
  h()
  return x
end
