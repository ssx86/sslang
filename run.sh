make &&

echo "\ntesting : syntax\n"
./sslang ./test/syntax.ss true

echo "\ntesting : lua\n"
./sslang ./test/lua.ss true

echo "\ntesting : case1\n"
./sslang ./test/case1.ss true

echo "\ntesting : tooshort\n"
./sslang ./test/tooshort.ss true

echo "\ntesting : eval\n"
./sslang ./test/eval.ss true
