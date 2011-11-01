#include "SSLHelper.h"

SSL_Connection *ssl_connect(const char *host, int port, const char *certfile, const char *keyfile, const char *capath)
{
	int err;

	printf("malloc\n");
	SSL_Connection *sslcon = NULL;
	sslcon = (SSL_Connection *)malloc(sizeof(SSL_Connection));
	if(sslcon == NULL)
	{
		printf("Could not allocate memory for SSL Connection");
		exit(1);
	}

	printf("SSL_library_init\n");
	/* Load encryption & hashing algorithms for the SSL program */
	SSL_library_init();

	printf("SSL_load_error_strings\n");
	/* Load the error strings for SSL & CRYPTO APIs */
	SSL_load_error_strings();

	printf("SSLv3_method\n");
	/* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
	sslcon->meth = SSLv3_method();

	printf("SSL_CTX_new\n");
	/* Create an SSL_CTX structure */
	sslcon->ctx = SSL_CTX_new(sslcon->meth);
	if(!sslcon->ctx)
	{
		printf("Could not get SSL Context\n");
		exit(1);
	}

	printf("SSL_CTX_load_verify_locations\n");
	/* Load the CA from the Path */
	if(SSL_CTX_load_verify_locations(sslcon->ctx, NULL, capath) <= 0)
	{
		/* Handle failed load here */
		printf("Failed to set CA location...\n");
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	printf("SSL_CTX_use_certificate_file\n");
	/* Load the client certificate into the SSL_CTX structure */
	if (SSL_CTX_use_certificate_file(sslcon->ctx, certfile, SSL_FILETYPE_PEM) <= 0) {
		printf("Cannot use Certificate File\n");
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	printf("SSL_CTX_use_PrivateKey_file\n");
	/* Load the private-key corresponding to the client certificate */
	if (SSL_CTX_use_PrivateKey_file(sslcon->ctx, keyfile, SSL_FILETYPE_PEM) <= 0) {
		printf("Cannot use Private Key\n");
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	printf("SSL_CTX_check_private_key\n");
	/* Check if the client certificate and private-key matches */
	if (!SSL_CTX_check_private_key(sslcon->ctx)) {
		printf("Private key does not match the certificate public key\n");
		exit(1);
	}

	printf("socket\n");
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	/* Set up a TCP socket */
	sslcon->sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sslcon->sock == -1)
	{
		printf("Could not get Socket (%i)\n", WSAGetLastError());
		exit(1);
	}

	memset (&sslcon->server_addr, '\0', sizeof(sslcon->server_addr));
	sslcon->server_addr.sin_family = AF_INET;
	sslcon->server_addr.sin_port = htons(port);	/* Server Port number */
	sslcon->host_info = gethostbyname(host);
	if(sslcon->host_info)
	{
		/* Take the first IP */
		struct in_addr *address = (struct in_addr*)sslcon->host_info->h_addr_list[0];
		sslcon->server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address));	/* Server IP */
	}
	else
	{
		printf("Could not resolve hostname %s\n", host);
		return NULL;
	}

	/* Establish a TCP/IP connection to the SSL client */
	err = connect(sslcon->sock, (struct sockaddr*) &sslcon->server_addr, sizeof(sslcon->server_addr));
	if(err == -1)
	{
		printf("Could not connect\n");
		exit(1);
	}

	/* An SSL structure is created */
	sslcon->ssl = SSL_new(sslcon->ctx);
	if(!sslcon->ssl)
	{
		printf("Could not get SSL Socket\n");
		exit(1);
	}

	/* Assign the socket into the SSL structure (SSL and socket without BIO) */
	SSL_set_fd(sslcon->ssl, sslcon->sock);

	/* Perform SSL Handshake on the SSL client */
	err = SSL_connect(sslcon->ssl);
	if(err == -1)
	{
		printf("Could not connect to SSL Server\n");
		exit(1);
	}

	return sslcon;
}

void ssl_disconnect(SSL_Connection *sslcon)
{
	int err;

	if(sslcon == NULL)
	{
		return;
	}

	/* Shutdown the client side of the SSL connection */
	err = SSL_shutdown(sslcon->ssl);
	if(err == -1)
	{
		printf("Could not shutdown SSL\n");
		exit(1);
	}

	/* Terminate communication on a socket */
	err = close(sslcon->sock);
	if(err == -1)
	{
		printf("Could not close socket\n");
		exit(1);
	}

	/* Free the SSL structure */
	SSL_free(sslcon->ssl);

	/* Free the SSL_CTX structure */
	SSL_CTX_free(sslcon->ctx);

	/* Free the sslcon */
	if(sslcon != NULL)
	{
		free(sslcon);
		sslcon = NULL;
	}
}
