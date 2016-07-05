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
    puts "[fail] #{basename}"
    $fail += 1
  end
end

puts "test done, #{$pass + $fail} cases, #{$pass} passed, #{$fail} failed, #{$pass.to_f / ($pass + $fail) * 100}%."
