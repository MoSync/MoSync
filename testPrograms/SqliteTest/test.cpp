#include <stdio.h>

void GiveMeTwo(const char* s1, const char* s2);
const char* GiveMeBack(const char* s);

const char* GiveMeBack(const char* s)
{
	printf("%s\n", s);
	return s;
}

void GiveMeTwo(const char* s1, const char* s2)
{
	printf("%s\n", s1);
	printf("%s\n", s2);
}

int main()
{
	GiveMeTwo(GiveMeBack("One"), GiveMeBack("Two"));
}
