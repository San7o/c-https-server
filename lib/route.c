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

#include <stddef.h>             /* NULL    */
#include <string.h>             /* strcpy  */
#include <stdlib.h>             /* malloc  */

#include <chttps/route.h>

chttps_error chttps_route_create(char* path, chttps_route **route)
{
  if (path == NULL || route == NULL)
    return -CHTTPS_ROUTE_IS_NULL_ERROR;
      
  *route = malloc(sizeof(chttps_route));
  strcpy((*route)->path, path);
  return CHTTPS_NO_ERROR;
}

/* Setters */

chttps_error chttps_route_set_get(chttps_route *route,
				    chttps_error (*get)(chttps_request *req, char *out))
{
    if (get == NULL || route == NULL)
	return -CHTTPS_ROUTE_IS_NULL_ERROR;

    route->get = get;
    return CHTTPS_NO_ERROR;
}

chttps_error chttps_route_set_head(chttps_route *route,
				   chttps_error (*head)(chttps_request *req, char *out))
{
    if (head == NULL || route == NULL)
	return -CHTTPS_ROUTE_IS_NULL_ERROR;

    route->head = head;
    return CHTTPS_NO_ERROR;
}

chttps_error chttps_route_set_post(chttps_route *route,
				   chttps_error (*post)(chttps_request *req, char *out))
{
    if (post == NULL || route == NULL)
	return -CHTTPS_ROUTE_IS_NULL_ERROR;

    route->post = post;
    return CHTTPS_NO_ERROR;
}

/* Getters */

chttps_error chttps_route_get(chttps_route *route,
			      chttps_request *req,
			      char* out)
{
    if (route == NULL || req == NULL)
	return -CHTTPS_ROUTE_IS_NULL_ERROR;

    return route->get(req, out);
}

chttps_error chttps_route_head(chttps_route *route,
			       chttps_request *req,
			       char* out)
{
  if (route == NULL || req == NULL)
    return -CHTTPS_ROUTE_IS_NULL_ERROR;
      
  return route->head(req, out);
}

chttps_error chttps_route_post(chttps_route *route,
			       chttps_request *req,
			       char* out)
{
  if (route == NULL || req == NULL)
    return -CHTTPS_ROUTE_IS_NULL_ERROR;
      
  return route->post(req, out);
}
