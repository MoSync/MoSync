#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <map>
#include <string>
#include "sld.h"

using namespace std;

typedef unsigned int uint;

struct DumpHeader {
	uint timeStamp;	// maGetMilliSecondCount()
	int requestedSize;	// number of bytes requested for malloc(). -1 for free().
	uint blockSize;	// number of bytes actually allocated/freed. 0 on malloc error.
	uint address;	// address of allocated block. 0 on malloc error.
	uint nFrames;	// number of stack frames
};

struct Dump : DumpHeader {
	uint* frames;
};

vector<Dump> sDumps;

struct MallocSet {
	MallocSet() : usedSize(0), nMallocs(0), nFrees(0) {}
	uint usedSize, nMallocs, nFrees;
};

struct StackComparator {
	typedef Dump Type;
	// true iff a < b;
	bool operator()(const Type& a, const Type& b) const {
		if(a.nFrames < b.nFrames)
			return true;
		if(a.nFrames > b.nFrames)
			return false;
		for(uint i=0; i<a.nFrames; i++) {
			if(a.frames[i] < b.frames[i])
				return true;
			if(a.frames[i] > b.frames[i])
				return false;
		}
		return false;
	}
};

struct MallocSetSizeComparator {
	typedef MallocSet Type;
	// true iff a < b;
	bool operator()(const Type& a, const Type& b) const {
		if(a.usedSize > b.usedSize)
			return true;
		else
			return false;
	}
};

// key: address. value: index into sDumps.
typedef map<uint, size_t> AddressMap;
static AddressMap sAddressMap;

typedef map<Dump, MallocSet, StackComparator> MallocMap;
static MallocMap sMallocMap;

typedef multimap<MallocSet, Dump, MallocSetSizeComparator> StackMap;

static bool forceRead(int fd, void* dst, int size, bool allowEof = false) {
	int res = read(fd, dst, size);
	if(res == 0 && allowEof)
		return false;
	if(res != size) {
		printf("read error %i %i(%s)\n", res, errno, strerror(errno));
		exit(1);
	}
	return true;
}

int main() {
	printf("Loading SLD...\n");
	if(!loadSLD("sld.tab")) {
		printf("Couldn't load sld.tab.\n");
		exit(1);
	}

	printf("Reading...\n");
	int fd = open("mhsd.bin", O_RDONLY | O_BINARY);
	char magic[4];
	forceRead(fd, magic, 4);
	if(memcmp(magic, "MHSD", 4) != 0) {
		printf("Not a valid MHSD file!\n");
		return 1;
	}

	Dump dump;
	int nMallocs=0, nFrees=0;
	while(forceRead(fd, &dump, sizeof(DumpHeader), true)) {
		dump.frames = new uint[dump.nFrames];
		forceRead(fd, dump.frames, dump.nFrames * sizeof(uint));

		// zero the blockSize of malloc dumps of freed items
		if(dump.requestedSize >= 0) {	//malloc
			//printf("malloc: 0x%x\n", dump.address);
			if(sAddressMap.find(dump.address) != sAddressMap.end()) {
				printf("After %i mallocs, %i frees:\n", nMallocs, nFrees);
				printf("Duplicate malloc: 0x%x\n", dump.address);
				exit(1);
			}
			sAddressMap[dump.address] = sDumps.size();
			nMallocs++;
		} else {	//free
			//printf("free: 0x%x\n", dump.address);
			AddressMap::iterator itr = sAddressMap.find(dump.address);
			if(itr == sAddressMap.end()) {
				printf("Broken free: 0x%x\n", dump.address);
				exit(1);
			}
			if(sDumps[itr->second].address != dump.address) {
				printf("Broken AddressMap: [i]: 0x%x != 0x%x\n",
					itr->second, sDumps[itr->second].address, dump.address);
				exit(1);
			}
			sDumps[itr->second].blockSize = 0;
			sAddressMap.erase(itr);
			nFrees++;
		}

		// store dump
		sDumps.push_back(dump);
	}

	printf("Read %i dumps.\n", sDumps.size());
	printf("%i(%i) mallocs. %i frees.\n", nMallocs, sAddressMap.size(), nFrees);

	// gather mallocs by stack
	for(size_t i=0; i<sDumps.size(); i++) {
		const Dump& d(sDumps[i]);
		if(d.requestedSize >= 0) {
			MallocSet& ms(sMallocMap[d]);
			ms.nMallocs++;
			ms.usedSize += d.blockSize;
			if(d.blockSize == 0)
				ms.nFrees++;
		}
	}

	printf("%i distinct malloc stacks.\n", sMallocMap.size());
	StackMap stackMap;
	for(MallocMap::const_iterator itr = sMallocMap.begin(); itr != sMallocMap.end(); itr++) {
		stackMap.insert(pair<MallocSet, Dump>(itr->second, itr->first));
	}

	printf("%i stacks sorted:\n", stackMap.size());
	for(StackMap::const_iterator itr = stackMap.begin(); itr != stackMap.end(); itr++) {
		const MallocSet& ms(itr->first);
		const Dump& d(itr->second);
		printf("%i bytes, %i mallocs, %i frees. %i frames:\n",
			ms.usedSize, ms.nMallocs, ms.nFrees, d.nFrames);
		for(uint i=0; i<d.nFrames; i++) {
			uint address = d.frames[i];
			int line;
			string file;
			mapIp(address, line, file);
			printf("%s:%i (0x%x)\n", file.c_str(), line, address);
		}
		printf("\n");
	}

	return 0;
}

void MoSyncErrorExit(int code) {
	printf("MoSyncErrorExit(%i)\n", code);
	exit(code);
}
