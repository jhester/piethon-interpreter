def f(x):
  if x < 10:
    x = x+1
    print x
    f(x)
  end
end

def g(x, y):
  if x == y:
    f(x)
  end
end

def h():
  x = 1
  y = 1
  g(x, y)
end

def main():
  h()
end
