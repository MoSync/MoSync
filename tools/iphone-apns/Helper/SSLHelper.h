#ifndef __SSL_HELPER_H
#define __SSL_HELPER_H

#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif	//WIN32

#include <unistd.h>

#include <sys/types.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct {
	/* SSL Vars */
	SSL_CTX* ctx;
	SSL* ssl;
	const SSL_METHOD* meth;
	X509* server_cert;
	EVP_PKEY* pkey;

	/* Socket Communications */
	struct sockaddr_in server_addr;
	struct hostent* host_info;
	int sock;
} SSL_Connection;

/* Prototypes */
SSL_Connection *ssl_connect(const char *host, int port, const char *cerfile, const char *keyfile, const char *capath);
void ssl_disconnect(SSL_Connection *sslcon);

#endif
