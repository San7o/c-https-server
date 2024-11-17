#!/bin/sh

# Use this script to generate an openssl certificate
# cert and key

mkdir certs 2>/dev/null || :
openssl req                   \
	-x509                 \
	-newkey rsa:4096      \
	-keyout certs/key.pem \
	-out certs/cert.pem   \
	-sha256               \
	-days 3650            \
	-nodes                \
	-subj "/C=XX/ST=StateName/L=CityName/O=CompanyName/OU=CompanySectionName/CN=CommonNameOrHostname"
