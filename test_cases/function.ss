function a(n)
  print(n)
end

function b(n)
  print(n * 2)
end

a(1)
b(1)


t = {
  a = 1,
  b = 2
}


t.c = 3

t.fun2 = function(x)
  print("in fun2");
  print(x)
end

t.c = 4

t.fun1 = function(x) 
  print("in fun1");
  print(x)
end

print("fun1 value : ")
t.fun1(1)

print("fun2 value : ")
t.fun2(4)


