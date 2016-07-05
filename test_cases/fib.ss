function fib(n)
  if n == 1 or n == 2 then
     return 1 
  else
     return fib(n-1) + fib(n-2)
  end
end

for i = 1, 10, 1 do 
  print(i .. ":" .. fib(i))
end

