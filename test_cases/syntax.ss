print(1 + 2 * 3 * 4 - 5 * 6 + 7 - 8 * 9)

print(1 + 2 * 3 + 4)

for i = 1, 5, 1 do 
    i = 3 + i
    i = i - 7
end

a = 13
b = 'Hello world'
i_1 = 9.0
b = 8
d = a + b
d = a - b
d = a * b
d = a / b

-- d = a ^ b
-- d = a | b
-- d = a || b
-- d = a && b
-- d = a & b


-- arr = {1, 2, 3, "ff"}
-- arr[0] = 4


function fib(a)
  if a <= 2 then
    return 1
  else
    return fib(a-1) + fib(a-2)
  end
end

print("===================\n")

for i = 1, 15 do 
	print(i .. ":\t" .. fib(i) )
end



