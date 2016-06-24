make &&
echo "-----------syntax------------" 
./sslang ./test/syntax.ss > output.txt

echo "-----------lua------------" 
./sslang ./test/lua.ss >> output.txt

echo "-----------tooshort------------" 
./sslang ./test/tooshort.ss >> output.txt
