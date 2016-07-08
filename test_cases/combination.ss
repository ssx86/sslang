function f(n)
  return n * 2
end


function g(n)
  return n + 1
end

function combination(f, g)
  return function(x)
    return f(g(x))
  end
end

h = combination(f, g)

print(h(5))
