/* 
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <chttps/ssl.h>

chttps_error chttps_ssl_create_context(SSL_CTX **ctx)
{
  if (!ctx)
    return -CHTTPS_SSL_CONTEXT_CREATION_ERROR;
  const SSL_METHOD *method;
  method = TLS_server_method();

  *ctx = SSL_CTX_new(method);
  if (!(*ctx))
    return -CHTTPS_SSL_CONTEXT_CREATION_ERROR;
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_ssl_configure_context(SSL_CTX *ctx, chttps_config *conf)
{
  char cert_path[CHTTPS_MAX_STRING_SIZE];
  strcpy(cert_path, conf->certs_dir);
  strcat(cert_path, "/cert.pem");
  if (SSL_CTX_use_certificate_file(ctx, cert_path, SSL_FILETYPE_PEM) <= 0)
    return -CHTTPS_SSL_CERTIFICATE_ERROR;
  strcpy(cert_path, conf->certs_dir);
  strcat(cert_path, "/key.pem");
  if (SSL_CTX_use_PrivateKey_file(ctx, cert_path, SSL_FILETYPE_PEM) <= 0)
    return -CHTTPS_SSL_KEY_ERROR;
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_ssl_connection(SSL_CTX *ctx,
				   chttps_client *client,
				   SSL **ssl)
{
  *ssl = SSL_new(ctx);
  SSL_set_fd(*ssl, client->cfd);
  if (SSL_accept(*ssl) <= 0)
    return -CHTTPS_SSL_ACCEPT_ERROR;
  return CHTTPS_NO_ERROR;
}
