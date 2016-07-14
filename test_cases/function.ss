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
  b = 2,
  c = {}
}



t.c.fun2 = function(x)
  print("in fun2");
  print(x)
end


t.c.fun1 = function(x) 
  print("in fun1");
  print(x)
end

print("fun1 value : ")
t.c.fun1(1)

print("fun2 value : ")
t.c.fun2(4)


