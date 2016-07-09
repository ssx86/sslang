l = {3, 5, 3, 5, 6, 4, 21, 1, 1}

function print_list(l)
  str = "";
  for i = 1, #l do 
    str = str .. l[i] .. ", "
  end
  print(str)
end

    
print_list(l)
for i = 1, #l, 1 do 
  for j = i, #l, 1 do 
    if (l[i] >= l[j]) then
      n = l[j]
      l[j] = l[i]
      l[i] = n
    end
  end
end

print_list(l)
