REM makekeys -cert -password password -len 2048 -dname "CN=  OU=  OR=  CO=  EM=default@example.com" my.key my.cer

openssl genrsa -rand -des -passout pass:password 1024 > my.key
openssl req -new -x509 -nodes -sha1 -days 3650 -key my.key -batch -config openssl.cnf > my.cert
