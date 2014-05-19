The cunning one is to make run-time loader to correctly do symbol
mapping. Since ld.so loader yet doesn't support direct binding
(http://en.wikipedia.org/wiki/Direct_binding) this can be done with
dlopen(RTLD_DEEPBIND) at SO init time. I've put some example in bug
900. Please, look at lib_vinegret.tar.gz.

You'll find in 'lib_vinegret' dir the simplified matter of problem:
lib_v1.c: Qt3 library
lib_v1_user.c: Most TDE code
lib_v2.c: Qt4 library
lib_v2_user.c: TDE code that wants to use Qt4

When you intermix 'lib_v1' and 'lib_v2' in one executable you will get
'lib_v2_user' executing 'lib_v1' instead of 'lib_v2'.
Please, type 'make', then './run.sh' to look at the results:

This is v1 user
This is v1 library!
This is v2 user
This is v1 library!

You can examine why this happened in generated rtld.log. The
interesting lines are:

     4885:     object=./lib_v1_user.so [0]
     4885:      scope 0: ./executable ./lib_v1_user.so
./lib_v2_user.so /lib/x86_64-linux-gnu/libc.so.6 ./lib_v1.so
./lib_v2.so /lib64/ld-linux-x86-64.so.2
     4885:
     4885:     object=./lib_v2_user.so [0]
     4885:      scope 0: ./executable ./lib_v1_user.so
./lib_v2_user.so /lib/x86_64-linux-gnu/libc.so.6 ./lib_v1.so
./lib_v2.so /lib64/ld-linux-x86-64.so.2

Note the 'scope 0'. It is same for all objects and called 'global
scope'. The order in this scope shows the actual order of symbol
searches. So the 'lib_function' symbol will be found always from
./lib_v1.so. Further entries in rtld.log confirm that.

Sadly enough, there is no way to force dynamic linker to load
different objects with different lookup scopes. But here comes
RTLD_DEEPBIND to the stage. Please, look at this diff:

http://bugs.pearsoncomputing.net/attachment.cgi?id=468&action=diff

I made some wrapper 'lib_v2_user_scoper' around the code we want to
load with different library. If you run this variant, you should see
this:

This is v1 user
This is v1 library!
This is v2 user
This is v2 library!
This is v1 user
This is v1 library!
This is v2 user
This is v2 library!

Voila! :-)

If per chance you will remove RTLD_DEEPBIND flag you will see the old
flat way of resolution:

This is v1 user
This is v1 library!
This is v2 user
This is v1 library!
This is v1 user
This is v1 library!
This is v2 user
This is v1 library!
