i, j = 3, 4 + 5
k = i + j
k = k * 3

h = k / 5;

d = i + j


function fib(n)
  if n == 1 then
     return 1
  elseif n == 2 then
     return 1 
  else
     return fib(n-1) + fib(n-2)
  end
end

for i = 1, 10, 1 do 
  print(fib(i))
  print(", ")
end

print("\n");
