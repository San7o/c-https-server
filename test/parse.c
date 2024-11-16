#include <chttps/chttps.h>
#include <string.h>

#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error in test parse: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)
#define handle_error(err) \
  do { fprintf(stderr, "Error in test parse: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

int main(void)
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
  return 0;
}
