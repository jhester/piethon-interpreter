def f(x):
  if x < 10:
    x = x+1
    print x
    f(x)
  end
end

def g(,x, y):
  if x == y:
    f(3)
  end
end

def h():
  x = 1
  y = 1
  g(5, 5)
end

def main():
  h()
end