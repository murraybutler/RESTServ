# RESTServ
requires cityhash and pistache.io

This is a Simple Bloom Filter, built as a shared object library, and used by both a unix domain socket,
and a REST based service to serve the filter.  They are "server" and "RESTServ", respectively, in the repo.
The accompanying Dockerfile is for an ubuntu 17.04 (zesty) machine that will build the required pistache
libraries, headers and project contained in this repo and produce a functioning system.

The Simple Bloom Filter is a basic design, utilizing Google's cityhash implementation, and Kirsch and
Mitzenmacher's ideas for simplifying hashing in a Bloom filter 
(https://www.eecs.harvard.edu/~michaelm/postscripts/tr-02-05.pdf).

The Server is a basic unix domain socket server to allow for IPC.  Initially, I had thought I would
need this to wrapper with the REST calls, but the pistache REST library can handle serving
on it's own.  In the interest of keeping the work in one place, I ahve left the unix server 
in the repo.

The REST implementation is a basic REST API for the BloomFilter.so library and simply utilizes the
class interfaces on an instance of the class.  I implemented BloomFilter.TestAndAdd() and BloomFilter.DumpStr()
to allow for visibility and use.  Simplistic, but effective.

I may add on the feature of allowing a call for a dump of the filter locally, and test for a local file 
to reload the filter as startup.  Consider that a todo.

Thanks for looking,

Murray Butler

