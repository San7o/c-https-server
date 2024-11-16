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
#include <stdio.h>               /* fprintf */

#define chttps_handle_error(err) \
  do { fprintf(stderr, "Error in test router_test: %s\n", chttps_err_str(-err)); \
    exit(EXIT_FAILURE); } while (0)
#define handle_error(err) \
  do { fprintf(stderr, "Error in test router_test: %s\n", err); \
    exit(EXIT_FAILURE); } while (0)

int main(void)
{
  chttps_error err;
  
  /* Create router */
  chttps_router router = chttps_router_init();

  /* Create some routes */
  chttps_route *route1 = NULL;
  chttps_route *route2 = NULL;
  chttps_route *route3 = NULL;
  err = chttps_route_create("/orders", &route1);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  err = chttps_route_create("/shop", &route2);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  err = chttps_route_create("/admin", &route3);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* Register the routes */
  err = chttps_router_add(&router, route1);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  err = chttps_router_add(&router, route2);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  err = chttps_router_add(&router, route3);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);

  /* checks */
  if (router.registered_routes != 3)
    handle_error("Number of registered routes is incorrect");

  chttps_route *match1;
  err = chttps_router_match(&router, "/admin", &match1);
  if (err != CHTTPS_NO_ERROR)
    chttps_handle_error(err);
  if (match1 != route3)
    handle_error("Match1: Routes do not match");

  chttps_route *match2;
  err = chttps_router_match(&router, "/profile", &match2);
  if (err == CHTTPS_NO_ERROR)
    handle_error("Match2: Result should be error");
  if (match2 != NULL)
    handle_error("Match2: Output should be NULL");

  /* Cleanup */
  chttps_router_free(&router);

  return 0;
}
