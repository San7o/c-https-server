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

#include <string.h>          /* strtok, strcpy */
#include <stdlib.h>          /* malloc         */
#include <stdio.h>           /* sfprintf       */

#include <chttps/parser.h>
#include <chttps/util.h>

chttps_error chttps_parse_request(char* request,
				  chttps_request **out)
{
  if (request == NULL || out == NULL)
    return -CHTTPS_PARSE_WRONG_METHOD_ERROR;

  /* Parse the request line */

  *out = malloc(sizeof(chttps_request));

  /* method */
  char* method = strtok(request, " ");
  if (method == NULL)
    return -CHTTPS_PARSE_WRONG_METHOD_ERROR;
  if (strcmp(method, "GET") == 0)
    {
      (*out)->header.method = CHTTPS_GET;
    }
  else if (strcmp(method, "POST") == 0)
    {
      (*out)->header.method = CHTTPS_POST;
    }
  else if (strcmp(method, "HEAD") == 0)
    {
      (*out)->header.method = CHTTPS_HEAD;
    }
  else return -CHTTPS_PARSE_WRONG_METHOD_ERROR;

  /* uri */
  char* uri = strtok(NULL, " ");
  if (uri == NULL)
    return -CHTTPS_PARSE_WRONG_URI_ERROR;
  if (strlen(uri) == 0)
    return -CHTTPS_PARSE_WRONG_URI_ERROR;
  strcpy((*out)->header.uri, uri);

  /* version */
  char *version = strtok(NULL, "\t\n");
  if (version == NULL)
    return -CHTTPS_PARSE_WRONG_VERSION_ERROR;
  if (strlen(version) != 8 || strncmp(version, "HTTP/", 5))
    return -CHTTPS_PARSE_WRONG_VERSION_ERROR;
  strncpy((*out)->header.version, version+5, 4);

  return CHTTPS_NO_ERROR;
}

chttps_error chttps_create_response(chttps_response *res, char** out)
{
  if (res == NULL || out == NULL)
    return -CHTTPS_RESPONSE_IS_NULL_ERROR;

  *out = malloc(sizeof(chttps_response_header)+res->body_len);

  /* Header */
  char code[4];
  sprintf(code, "%d", res->header.status_code);

  strcpy(*out, "HTTP/");
  strcat(*out, res->header.version);
  strcat(*out, " ");
  strcat(*out, code);
  strcat(*out, " ");
  strcat(*out, chttps_code_str(res->header.status_code));
  strcat(*out, "\t\n");

  /* Body */
  strncat(*out, res->body, res->body_len);

  return CHTTPS_NO_ERROR;
}
