l = {'aa', 'bb', 3, 4, 5, [3] = 1 + 5, a = 3 + 7}

l.a = 15
print('the length of l is ' .. #l);

--print(l)
print(l.a)

print(l[1])
print(l[2])
print("============")
print(l['a'])
print(l[a])
