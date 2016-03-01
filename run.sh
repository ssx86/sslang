echo "-----------case1------------"
cat ./test/case1.ss && make && ./sslang ./test/case1.ss
echo "-----------case2------------"
cat ./test/case2.ss && make && ./sslang ./test/case2.ss
