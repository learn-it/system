#include <map>
#include <algorithm>
#include <string>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include <iostream>

using namespace std;

// items are stored until this count of seconds
static size_t cache_ttl = 2;

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

map_byname by_name;
map_bytime by_time;

struct delete_name
{
    void operator() (map_bytime::value_type& n)
    {
	by_name.erase(n.second);
    }
} name_deleter;

class RWLock
{
public:
    RWLock() { pthread_rwlock_init(&lock_, 0); }
    ~RWLock() { pthread_rwlock_destroy(&lock_); }
    operator pthread_rwlock_t&() { return lock_; }

private:
    pthread_rwlock_t lock_;
};

class RGuard
{
public:
    RGuard(pthread_rwlock_t& l): lock_(l) { pthread_rwlock_rdlock(&lock_); }
    ~RGuard() { pthread_rwlock_unlock(&lock_); }

private:
    pthread_rwlock_t& lock_;
};

class WGuard
{
public:
    WGuard(pthread_rwlock_t& l): lock_(l) { pthread_rwlock_wrlock(&lock_); }
    ~WGuard() { pthread_rwlock_unlock(&lock_); }

private:
    pthread_rwlock_t& lock_;
};

static RWLock lock;


// performance ~1.1m ops/sec
int check_negcache(const char *url)
{
    if (*url != '/')
	return 0; /* (won't cache unknown stuff) */

    ++url; // skip leading '/'
    if (*url == 0)
	return 0; /* (something evil is happening) */

    RGuard guard(lock);
    if (by_time.size() == 0)
	return 0;

    string host(url, strchrnul(url, '/'));

    time_t expiry;
    time(&expiry);
    expiry -= cache_ttl;
    
    it_bytime t = by_time.end();
    it_bytime p = t--;
    while (true)
    {
	if (t->first <= expiry) {
	    for_each(by_time.begin(), p, name_deleter);
	    by_time.erase(by_time.begin(), p);
	    break;
	}
	if (t == by_time.begin())
            break;
	p = t--;
    }

    it_byname name_it = by_name.find(host);
    if (name_it == by_name.end())
	return 0;

    return name_it->second.error;
}

// performance ~850k ops/sec
void set_negcache(const char *url, int error)
{
    if (*url != '/')
	return; /* (won't cache unknown stuff) */

    if (error != ECONNREFUSED && error != ETIMEDOUT)
	return;

    ++url; // skip leading '/'
    if (*url == 0)
	return; /* (something evil is happening) */

    string host(url, strchrnul(url, '/'));
    WGuard guard(lock);
    it_byname name_it = by_name.find(host);
    it_bytime time_it;

    if (name_it != by_name.end()) {
	*(void **)&time_it = name_it->second.time_it;
	by_time.erase(time_it);
	name_it->second.error = error;
    } else {
	pair<it_byname, bool> res = by_name.insert(
	    map_byname::value_type(host, error)
	);
	name_it = res.first;
    }
    
    time_it = by_time.insert(map_bytime::value_type(time(0), name_it));
    name_it->second.time_it = *(void **)&time_it;
}
