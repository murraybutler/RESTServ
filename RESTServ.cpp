/* 
   Mathieu Stefani, 07 février 2016
   
   Example of a REST endpoint with routing
*/

#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "include/BloomFilter.h"

using namespace std;
using namespace Pistache;

namespace Generic {

void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "1");
}

}

class DedupEndpoint {
public:
    DedupEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    {}

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void shutdown() {
        httpEndpoint->shutdown();
    }

    string reply = "";
private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/test", Routes::bind(&DedupEndpoint::doFiltTestAdd, this));
        Routes::Get(router, "/dump", Routes::bind(&DedupEndpoint::doFiltDump, this));

    }

    void doFiltTestAdd(const Rest::Request& request, Http::ResponseWriter response) {
        auto elem = request.body();
	bool ret = false;

	ret = bf->TestAndAdd(elem);
        if (ret) {
	    reply = "String Exists\n";
        } else {
	    reply = "String Added\n";
	}
        response.send(Http::Code::Created, reply);

	    
    }

    void doFiltDump(const Rest::Request& request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, bf->DumpStr());

    }

    class LocalFilt {
    public:
	BloomFilter& getBloomFilter()
	{
	    static BloomFilter bf;
	    return bf;
	}
    };

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
    BloomFilter *bf = new BloomFilter();

};

int main(int argc, char *argv[]) {
    Port port(9080);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    DedupEndpoint dedup(addr);

    dedup.init(thr);
    dedup.start();

    dedup.shutdown();
}
