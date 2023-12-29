gcc -O0 -o test -g -finstrument-functions test.c -L. -lpthread -ldl -lhooks -ldinfo
