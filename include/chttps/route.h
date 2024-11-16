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

#ifndef _CHTTPS_ROUTE_H
#define _CHTTPS_ROUTE_H

#include <chttps/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new route with given path
 */
chttps_error chttps_route_create(char* path, chttps_route **route);

/* Setters */
chttps_error chttps_route_set_get(chttps_route *route,
				  chttps_error (*get)(chttps_request *req, char **out));
chttps_error chttps_route_set_head(chttps_route *route,
				   chttps_error (*get)(chttps_request *req, char **out));
chttps_error chttps_route_set_post(chttps_route *route,
				   chttps_error (*get)(chttps_request *req, char **out));

/* Getters */
chttps_error chttps_route_get(chttps_route *route,
			      chttps_request *req,
			      char** out);
chttps_error chttps_route_head(chttps_route *route,
			       chttps_request *req,
			       char** out);
chttps_error chttps_route_post(chttps_route *route,
			       chttps_request *req,
			       char** out);
  
#ifdef __cplusplus
}
#endif

#endif
