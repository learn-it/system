shared := -shared -fPIC
rpath := -Xlinker -rpath=.

all: executable

lib_v1.so: lib_v1.c
	gcc $(shared) $< -o $@

lib_v2.so: lib_v2.c
	gcc $(shared) $< -o $@

lib_v1_user.so: lib_v1_user.c lib_v1.so
	gcc $(shared) $(rpath) $^ -o $@

lib_v2_user.so: lib_v2_user.c lib_v2.so
	gcc $(shared) $(rpath) $^ -o $@

executable: executable.c lib_v1_user.so lib_v2_user.so
	gcc $(rpath) $^ -o $@

clean:
	rm -f *.so *.log executable
