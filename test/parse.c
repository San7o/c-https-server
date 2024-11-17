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

#include <chttps/chttps.h>
#include <string.h>
#include <stdio.h>

#define handle_error(err) \
  do { fprintf(stderr, "Error in test parse: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

void parse_request(void)
{
  chttps_request *req;
  chttps_error err;
  char line[100];

  strcpy(line, "GET /index.html HTTP/1.0\t\n");
  err = chttps_parse_request(line, &req);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  if (req == NULL)
    handle_error("Test 1: Request is null");
  if (req->header.method != CHTTPS_GET)
    handle_error("Test 1: Wrong method");
  if (strcmp(req->header.uri, "/index.html\0") != 0)
    handle_error("Test 1: Wrong uri");
  if (strcmp(req->header.version, "1.0\0") != 0)
    handle_error("Test 1: Wrong version");
  free(req);

  strcpy(line, "POST /api/v2/post HTTP/1.0\t\n");
  err = chttps_parse_request(line, &req);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  if (req == NULL)
    handle_error("Test 2: Request is null");
  if (req->header.method != CHTTPS_POST)
    handle_error("Test 2: Wrong method");
  if (strcmp(req->header.uri, "/api/v2/post\0") != 0)
    handle_error("Test 2: Wrong uri");
  if (strcmp(req->header.version, "1.0\0") != 0)
    handle_error("Test 2: Wrong version");
  free(req);

  strcpy(line, "HEAD /admin.php HTTP/1.0\t\n");
  err = chttps_parse_request(line, &req);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  if (req == NULL)
    handle_error("Test 3: Request is null");
  if (req->header.method != CHTTPS_HEAD)
    handle_error("Test 3: Wrong method");
  if (strcmp(req->header.uri, "/admin.php\0") != 0)
    handle_error("Test 3: Wrong uri");
  if (strcmp(req->header.version, "1.0\0") != 0)
    handle_error("Test 3: Wrong version");
  free(req);

  strcpy(line, "HEADD /admin.php HTTP/1.0\t\n");
  err = chttps_parse_request(line, &req);
  if (-err != CHTTPS_PARSE_WRONG_METHOD_ERROR)
    handle_error("Test: wrong method failed");

  strcpy(line, "POST /adm in.php HTTP/1.0\t\n");
  err = chttps_parse_request(line, &req);
  if (-err != CHTTPS_PARSE_WRONG_VERSION_ERROR)
    handle_error("Test: wrong version failed");
  return;
}

void response_string(void)
{
  chttps_response res = {};
  res.header.status_code = 200;
  strcpy(res.header.version, "1.0");
  res.body_len = 5;
  res.body = malloc(6);
  strcpy(res.body, "hello");

  chttps_error err;
  char *out;
  err = chttps_create_response(&res, &out);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  char expected[] = "HTTP/1.0 200 OK\t\nhello";
  if (strncmp(out, expected, strlen(expected)) != 0)
    handle_error("Test 1: Wrong response");
  free(out);
  return;
}

int main(void)
{
  parse_request();
  response_string();
  return 0;
}
