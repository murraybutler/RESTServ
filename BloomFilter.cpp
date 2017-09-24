#include <string>
#include <fstream>
#include <stdint.h>
#include "include/BloomFilter.h"
#include "include/city.h"

BloomFilter::BloomFilter()
:pbv(NULL) {
    this->pbv = new BV();
    this->pbv->reset();
}

BloomFilter::~BloomFilter() {
    if(this->pbv)
    {
        delete pbv;	
    }
}

void BloomFilter::Dump(const char* out_path) {
    ::std::ofstream out(out_path);
    out<<this->pbv->to_string();
    out.close();
}

::std::string BloomFilter::DumpStr() {
    ::std::string bfout;
    bfout = this->pbv->to_string();
    return bfout;
}

void BloomFilter::Add(const ::std::string key) {
    uint32_t seed = 0;
    for(size_t i=0; i<M; i++)
    {
        uint32_t pos = 0;
        pos = CityHash32(key.c_str(), key.length());
        seed = pos;
        pos %= LEN;
        this->pbv->set(pos);
    }
}

bool BloomFilter::Test(const ::std::string key) {
    uint32_t seed = 0;
    for(size_t i=0; i<M; i++)
    {
        uint32_t pos = 0;
        pos = CityHash32(key.c_str(), key.length());
        seed = pos;
        pos %= LEN;
        if(!this->pbv->test(pos))
        {
            return false;
        }
    }
	return true;
}

bool BloomFilter::TestAndAdd(const ::std::string key) {
    if(this->Test(key))
    {
	return true;	
    } else {
	this->Add(key);
	return false;
    }
}
