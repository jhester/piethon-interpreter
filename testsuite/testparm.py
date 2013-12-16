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
  g(1, 1)
end

def main():
  h()
end
