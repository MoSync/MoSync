#include "Location.h"
#include "File.h"

using namespace std;

Location::Location(int line, const File* file) : mLine(line), mFile(file) {
}

int Location::getLine() const {
	return mLine;
}

const File* Location::getFile() const {
	return mFile;
}

string Location::toString() const {
	return System::genstr("%d:%s", mLine, mFile->toString().c_str());
}
