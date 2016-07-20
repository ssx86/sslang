Y = function (f)
   return function(a)
      return (function(x) return x(x) end)(function(x) return f(function(y) return x(x)(y) end) end)(a)
   end
end
 


almostfactorial = function(f) 
  return function(n) 
    if n > 0 then 
      return n * f(n-1) 
    else 
      return 1 
    end 
  end 
end

almostfibs = function(f) 
  return function(n) 
    if n < 2 then 
      return n 
    else 
      return f(n-1) + f(n-2) 
    end
  end
end
factorial, fibs = Y(almostfactorial), Y(almostfibs)
print(fibs(7))
print(factorial(7))
