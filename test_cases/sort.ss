l = {3, 5, 3, 5, 6, 4, 21, 1, 1}

for i = 1, #l, 1 do 
  print('i = ' .. i)
  for j = i, #l, 1 do 
  print('j = ' .. j)
    if (l[i] >= l[j]) then
      n = l[j]
      l[j] = l[i]
      l[i] = n
    end
  end
end

print(l)
