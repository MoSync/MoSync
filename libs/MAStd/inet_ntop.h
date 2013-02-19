#ifndef INET_NTOP_H
#define INET_NTOP_H

#ifdef __cplusplus
extern "C" {
#endif

#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46
const char* inet_ntop4(const byte* src, char* dst, size_t size);
const char* inet_ntop6(const byte* src, char* dst, size_t size);

#ifdef __cplusplus
}
#endif

#endif	//INET_NTOP_H
