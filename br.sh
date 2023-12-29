cp ~/dinfo/target/release/libdinfo.so ./
gcc -shared -finstrument-functions -fPIC hook.c -o libhooks.so
gcc -O0 -o test -g -finstrument-functions test.c -L. -lpthread -ldl -lhooks -ldinfo
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./test
