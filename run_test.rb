require 'find'

puts "running test:"

$pass = 0;
$fail = 0;

Find.find('./test_cases') do |file|
  next if File.directory? file

  basename = File.basename(file, ".ss")

  sslang_output = `./sslang #{file}`
  lua_output = `lua #{file}`

  if sslang_output == lua_output then
    puts "[pass] #{basename}"
    $pass += 1
  else
    puts "[\033[5mfail\033[0m] #{basename}"
    $fail += 1
  end
end

if $fail > 0 then
  puts "\ntest done, #{$pass + $fail} cases, #{$pass} passed, \033[31m#{$fail} failed\033[0m, \033[33m#{$pass * 100 / ($pass + $fail)}%\033[0m."
else
  puts "\ntest done, #{$pass + $fail} cases, all passed."
end
