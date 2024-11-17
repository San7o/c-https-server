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

#include <stddef.h>            /* NULL          */
#include <stdlib.h>            /* malloc & free */
#include <string.h>            /* strcmp        */

#include <chttps/router.h>

chttps_router chttps_router_init()
{
  chttps_router router = {};
  router.registered_routes = 0;
  return router;
}

chttps_error chttps_router_free(chttps_router *router)
{
  if (router == NULL)
    return -CHTTPS_ROUTER_IS_NULL_ERROR;

  for (size_t i = 0; i < router->registered_routes; ++i)
    free(router->routes[i]);
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_router_add(chttps_router *router,
			       chttps_route *route)
{
  if (router == NULL)
    return -CHTTPS_ROUTER_IS_NULL_ERROR;
  if (route == NULL)
    return -CHTTPS_ROUTE_IS_NULL_ERROR;

  if (router->registered_routes < CHTTPS_MAX_ROUTES)
    {
      router->routes[router->registered_routes] = route;
      ++router->registered_routes;
      return CHTTPS_NO_ERROR;
    }
  return -CHTTPS_ROUTER_FULL_ERROR;
}

chttps_error chttps_router_match(chttps_router *router,
		                 char* path, chttps_route **result)
{
  if (router == NULL)
    {
      *result = NULL;
      return -CHTTPS_ROUTER_IS_NULL_ERROR;
    }
  if (path == NULL)
    {
      *result = NULL;
      return -CHTTPS_PARSE_WRONG_URI_ERROR;
    }
  if (result == NULL)
    {
      *result = NULL;
      return -CHTTPS_ROUTE_IS_NULL_ERROR;
    }
  
  for (size_t i = 0; i < router->registered_routes; ++i)
    if (strcmp(router->routes[i]->path, path) == 0)
      {
	*result = router->routes[i];
	return CHTTPS_NO_ERROR;
      }

  *result = NULL;
  return -CHTTPS_ROUTER_NO_MATCH_ERROR;
}
