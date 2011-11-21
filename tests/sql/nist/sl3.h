#define AUTHID(a)

struct SqlSelectResultsStruct;

typedef struct SqlSelectResultsStruct* SqlSelectResults;

void sqlExec(const char* sqlString);
SqlSelectResults sqlSelect(const char* sqlString);
long sqlGetLong(SqlSelectResults, int index);
void sqlFinalize(SqlSelectResults ssr);
