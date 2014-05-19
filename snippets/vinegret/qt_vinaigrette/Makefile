rpath := -Xlinker -rpath=.
shared := -shared -fPIC


all: executable

qt3_module.so: qt3_module.cpp
	g++ $(shared) $(rpath) -g -I /usr/include/qt3 $< /usr/lib/libqt-mt.so -o $@

qt4_module.so: qt4_module.cpp
	g++ $(shared) $(rpath) -g -I /usr/include/qt4 $< /usr/lib/x86_64-linux-gnu/libQtCore.so.4.8.0 -o $@

qt4_module_scoper.so: qt4_module_scoper.cpp qt4_module.so
	g++ $(shared) $(rpath) -g $< -ldl -o $@

executable: executable.cpp qt3_module.so qt4_module_scoper.so
	g++ $(rpath) -g $^ -o $@

clean:
	rm executable *.so *.o *.log
