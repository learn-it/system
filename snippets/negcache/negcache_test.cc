#include "neg_cache.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

struct data_byname
{
    data_byname(int err): time_it(0), error(err) {}
    void * time_it;
    int error;
};

typedef map<string, data_byname> map_byname;
typedef map_byname::iterator it_byname;
typedef multimap<time_t, it_byname> map_bytime;
typedef map_bytime::iterator it_bytime;

extern map_byname by_name;
extern map_bytime by_time;


class Exercise
{
protected:
    virtual void iterate() = 0;
    const char * name;

    Exercise(const char* n) : name(n) {}

public:
    void run()
    {
        timeval tv_start, tv_end;
        gettimeofday(&tv_start, NULL);
        size_t iters = 1000000;
        for (int i = 0; i < iters; ++i) {
            iterate();
        }
        gettimeofday(&tv_end, NULL);
        suseconds_t elapsed = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 + (tv_end.tv_usec - tv_start.tv_usec);
        printf("Elapsed %ld usecs\n", elapsed);
        printf("%s: %d ops/sec\n", name, (int) ((double) 1000000 / elapsed * iters));
    }
};


class ExerciseSetNegcache : public Exercise
{
    virtual void iterate()
    {
        set_negcache("/perftest/xyz", 110);
    }

public:
    ExerciseSetNegcache() : Exercise("set_negcache") {}
};


class ExerciseCheckNegcache : public Exercise
{
    virtual void iterate()
    {
        check_negcache("/perftest/abc");
    }

public:
    ExerciseCheckNegcache() : Exercise("check_negcache") {}
};

class NegCacheUtil
{
public:
    void print()
    {
        for (it_byname n = by_name.begin(); n != by_name.end(); ++n)
        {
            it_bytime t;
            *(void **)& t = n->second.time_it;
            if (t->second != n) {
                cerr << "Wrong it_bytime data -- not an iterator to it_byname!\n";
                exit(1);
            }
            cout << n->first << " " << t->first << endl;
        }

        for (it_bytime t = by_time.begin(); t != by_time.end(); ++t)
        {
            it_byname n = t->second;
            cout << t->first << " " << n->first << endl;
        }
    }
};

int main()
{
    NegCacheUtil util;
    set_negcache("/abc", 110);
    set_negcache("/def", 110);
    set_negcache("/xyz", 110);
    set_negcache("/def", 110);
    set_negcache("/yum", 100);
    util.print();
    sleep(2);
    cout << endl << "*** 2 seconds later ***" << endl;
    set_negcache("/2 seconds later", 110);
    set_negcache("/abc updated", 110);
    set_negcache("/abc", 110);
    util.print();
    cout << endl << "*** after cleanup ***" << endl;
    bool res = check_negcache("/abc");
    cout << "true: " << res << endl;
    res = check_negcache("/xyz");
    cout << "false: " << res << endl;
    util.print();

    cout << endl << "*** performance ***" << endl;
    ExerciseSetNegcache ex1;
    ex1.run();

    ExerciseCheckNegcache ex2;
    ex2.run();
    return 0;
}
