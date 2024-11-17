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

#include <stdlib.h>     /* atol */

#include <chttps/daemon/args.h>

void chttps_print_banner(void)
{
  char* banner =
    "/*======================================================*\n"
    " *                    CHTTPS v0.0.1                     *\n"
    " *======================================================*/\n";
  printf(banner);
  return;
}

void chttps_print_help(void)
{
  chttps_print_banner();
  printf("Usage:\n");
  printf("  chttpsd [args...]\n\n");
  printf("Accepted args:\n");
  printf("    --port,-p\n");
  printf("        select port to listen to (default is 1234)\n");
  printf("    --ip,-i\n");
  printf("        select ip to listen to (default is 0.0.0.0)\n");
  printf("    --log-level,-ll\n");
  printf("        select the output log level. Options are:\n");
  printf("        - DEBUG\n");
  printf("        - INFO (DEFAULT)\n");
  printf("        - WARNING\n");
  printf("        - ERROR\n");
  printf("        - OUTPUT\n");
  printf("        - DISABLED\n");
  printf("    --no-banner,-np\n");
  printf("        do not show banner at startup (default true)\n");
  printf("    --help,-h\n");
  printf("        show this message\n");
  exit(0);
}

chttps_error chttps_parse_args(int argc, char**argv, chttps_config *conf)
{
  char *arg;
  for (int i = 1; i < argc; ++i)
    {
      char *arg = argv[i];
      if ((strcmp(arg, "--port") == 0)
	  || (strcmp(arg, "-p") == 0))
	{
	  if (i + 1 < argc)
	    {
	      char* port_c = argv[i+1];
	      if (port_c == NULL)
		return -CHTTPS_INVALID_ARGUMENT_PORT_ERROR;
	      chttps_port_t port = atol(port_c);
	      if (port == 0)
		return -CHTTPS_INVALID_ARGUMENT_PORT_ERROR;
	      conf->port = port;
	      ++i;
	    }
	  else
	    {
	      return -CHTTPS_MISSING_ARGUMENT_PORT_ERROR;
	    }
	}
      else if ((strcmp(arg, "--ip") == 0) ||
	       (strcmp(arg, "-i") == 0))
	{
	  if (i + 1 < argc)
	    {
	      char* ip = argv[i+1];
	      if (ip == NULL)
		return -CHTTPS_INVALID_ARGUMENT_IP_ERROR;
	      conf->listen_ip = ip;
	      ++i;
	    }
	  else
	    {
	      return -CHTTPS_MISSING_ARGUMENT_IP_ERROR;
	    }
	}
      else if ((strcmp(arg, "--log-level") == 0) ||
	       (strcmp(arg, "-ll") == 0))
	{
	  if (i + 1 < argc)
	    {
	      char* log_level = argv[i+1];
	      if (log_level == NULL)
		return -CHTTPS_INVALID_ARGUMENT_LOG_LEVEL_ERROR;
	      bool found = false;
	      for (int l = 0; l < _CHTTPS_LOG_LEVEL_MAX; ++l)
		{
		  if (strcmp(log_level, chttps_log_level_str((chttps_log_level) l)) == 0)
		    {
		      conf->log_level = (chttps_log_level) l;
		      found = true;
		      break;
		    }
		}
	      if (found == false)
		return -CHTTPS_INVALID_ARGUMENT_LOG_LEVEL_ERROR;
	      ++i;
	    }
	  else
	    {
	      return -CHTTPS_MISSING_ARGUMENT_LOG_LEVEL_ERROR;
	    }
	}
      else if ((strcmp(arg, "--no-banner") == 0) ||
	       (strcmp(arg, "-nb") == 0))
	{
	  conf->show_banner = false;
	}
      else if ((strcmp(arg, "--help") == 0) ||
	       (strcmp(arg, "-h") == 0))
	{
	  chttps_print_help();
	}
      else {
	chttps_print_help();
	return -CHTTPS_UNKNOWN_ARGUMENT_ERROR;
      }
    }
  return CHTTPS_NO_ERROR;
}
