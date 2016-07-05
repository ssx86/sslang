function call( fun, b) 
  fun(b)
end

function call2( fun, a, b)
  fun(a .. '=' ,  b / 2)
end

call(function (x) print(x*2) end, 3)
call2(function(x, y) print(x .. y) end, 100, 200)


  
  
