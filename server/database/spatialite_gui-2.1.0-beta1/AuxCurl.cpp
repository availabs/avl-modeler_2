/*
/ AuxCurl.cpp
/ implementing operations based on Curl
/
/ version 2.1, 2018 August 1
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2018  Alessandro Furieri
/
/    This program is free software: you can redistribute it and/or modify
/    it under the terms of the GNU General Public License as published by
/    the Free Software Foundation, either version 3 of the License, or
/    (at your option) any later version.
/
/    This program is distributed in the hope that it will be useful,
/    but WITHOUT ANY WARRANTY; without even the implied warranty of
/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/    GNU General Public License for more details.
/
/    You should have received a copy of the GNU General Public License
/    along with this program.  If not, see <http://www.gnu.org/licenses/>.
/
*/

#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "Classdef.h"

#include <curl/curl.h>

#include "config.h"

#ifdef SPATIALITE_AMALGAMATION
#include <spatialite/sqlite3.h>
#else
#include <sqlite3.h>
#endif

#include <spatialite.h>

#include "AuxCurl.h"

typedef struct curlMemBufferStruct
{
/* a struct handling a dynamically growing output buffer */
  unsigned char *Buffer;
  size_t WriteOffset;
  size_t BufferSize;
  int Error;
} curlMemBuffer;
typedef curlMemBuffer *curlMemBufferPtr;

static void curlMemBufferInitialize(curlMemBufferPtr buf)
{
// initializing a dynamically growing output buffer 
  buf->Buffer = NULL;
  buf->WriteOffset = 0;
  buf->BufferSize = 0;
  buf->Error = 0;
}

static void curlMemBufferReset(curlMemBufferPtr buf)
{
// cleaning a dynamically growing output buffer
  if (buf->Buffer)
    free(buf->Buffer);
  buf->Buffer = NULL;
  buf->WriteOffset = 0;
  buf->BufferSize = 0;
  buf->Error = 0;
}

static void
curlMemBufferAppend(curlMemBufferPtr buf, const unsigned char *payload,
                    size_t size)
{
// appending into the buffer
  size_t free_size = buf->BufferSize - buf->WriteOffset;
  if (size > free_size)
    {
      // we must allocate a bigger buffer
      size_t new_size;
      unsigned char *new_buf;
      if (buf->BufferSize == 0)
        new_size = size + 1024;
      else if (buf->BufferSize <= 4196)
        new_size = buf->BufferSize + size + 4196;
      else if (buf->BufferSize <= 65536)
        new_size = buf->BufferSize + size + 65536;
      else
        new_size = buf->BufferSize + size + (1024 * 1024);
      new_buf = (unsigned char *) malloc(new_size);
      if (!new_buf)
        {
          buf->Error = 1;
          return;
        }
      if (buf->Buffer)
        {
          memcpy(new_buf, buf->Buffer, buf->WriteOffset);
          free(buf->Buffer);
        }
      buf->Buffer = new_buf;
      buf->BufferSize = new_size;
    }
  memcpy(buf->Buffer + buf->WriteOffset, payload, size);
  buf->WriteOffset += size;
}

static size_t
curlStoreData(char *ptr, size_t size, size_t nmemb, void *userdata)
{
// updating the dynamic buffer
  size_t total = size * nmemb;
  curlMemBufferAppend((curlMemBufferPtr) userdata, (unsigned char *) ptr,
                      total);
  return total;
}

static void
curlCheckHttpHeader(curlMemBufferPtr buf, int *http_status, char **http_code)
{
// checking the HTTP header 
  unsigned char *p_in;
  unsigned char *base_status;
  unsigned char *base_code;
  int size_status = 0;
  int size_code = 0;
  char *tmp;

  *http_status = -1;
  *http_code = NULL;
  if (buf->Buffer == NULL)
    return;
  if (buf->WriteOffset < 10)
    return;
  if (memcmp(buf->Buffer, "HTTP/1.1 ", 9) != 0
      && memcmp(buf->Buffer, "HTTP/1.0 ", 9) != 0)
    return;

// attempting to retrieve the HTTP status 
  p_in = buf->Buffer + 9;
  base_status = p_in;
  while ((size_t) (p_in - buf->Buffer) < buf->WriteOffset)
    {
      if (*p_in == ' ')
        break;
      size_status++;
      p_in++;
    }
  if (size_status <= 0)
    return;
  tmp = (char *) malloc(size_status + 1);
  memcpy(tmp, base_status, size_status);
  *(tmp + size_status) = '\0';
  *http_status = atoi(tmp);
  free(tmp);

// attempting to retrieve the HTTP code 
  p_in = buf->Buffer + 10 + size_status;
  base_code = p_in;
  while ((size_t) (p_in - buf->Buffer) < buf->WriteOffset)
    {
      if (*p_in == '\r')
        break;
      size_code++;
      p_in++;
    }
  if (size_code <= 0)
    return;
  tmp = (char *) malloc(size_code + 1);
  memcpy(tmp, base_code, size_code);
  *(tmp + size_code) = '\0';
  *http_code = tmp;
}

extern char *GetUpdateVersion()
{
//
// checking if there is an updated version
//
  CURL *curl = NULL;
  CURLcode res;
  int http_status;
  char *http_code;
  char *text = NULL;
  curlMemBuffer headerBuf;
  curlMemBuffer bodyBuf;
  char *request;
  int windows;
#ifdef _WIN32
  windows = 1;
#else
  windows = 0;
#endif
  request =
    sqlite3_mprintf
    ("http://www.gaia-gis.it/cgi-bin/splitegui_update?windows=%d&cpu=%s&version=%s",
     windows, spatialite_target_cpu(), VERSION);
  curl = curl_easy_init();

  if (curl)
    {
      /* setting the URL */
      curl_easy_setopt(curl, CURLOPT_URL, request);

      /* no progress meter please */
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
      /* setting the output callback function */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlStoreData);

      /* initializes the buffers */
      curlMemBufferInitialize(&headerBuf);
      curlMemBufferInitialize(&bodyBuf);
      curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &headerBuf);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bodyBuf);

      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if (res != CURLE_OK)
        {
          fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
          goto stop;
        }

      /* verifying the HTTP status code */
      curlCheckHttpHeader(&headerBuf, &http_status, &http_code);
      if (http_status != 200)
        {
          fprintf(stderr, "Invalid HTTP status code: %d %s\n",
                  http_status, http_code);
          if (http_code != NULL)
            free(http_code);
          goto stop;
        }
      if (http_code != NULL)
        free(http_code);

      curlMemBufferReset(&headerBuf);
      text = (char *) malloc(bodyBuf.WriteOffset + 1);
      memcpy(text, (const char *) (bodyBuf.Buffer), bodyBuf.WriteOffset);
      *(text + bodyBuf.WriteOffset) = '\0';

    stop:
      curlMemBufferReset(&headerBuf);
      curlMemBufferReset(&bodyBuf);
      curl_easy_cleanup(curl);
    }

  sqlite3_free(request);
  return text;
}

extern bool DoDownloadUpdatedPackage(const char *download_url,
                                     unsigned char **data, int *data_len)
{
//
// downloading the updated version package
//
  CURL *curl = NULL;
  CURLcode res;
  int http_status;
  char *http_code;
  curlMemBuffer headerBuf;
  curlMemBuffer bodyBuf;
  bool retcode = false;

  *data = NULL;
  *data_len = 0;
  curl = curl_easy_init();

  if (curl)
    {
      /* setting the URL */
      curl_easy_setopt(curl, CURLOPT_URL, download_url);

      /* no progress meter please */
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
      /* setting the output callback function */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlStoreData);

      /* initializes the buffers */
      curlMemBufferInitialize(&headerBuf);
      curlMemBufferInitialize(&bodyBuf);
      curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &headerBuf);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bodyBuf);

      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if (res != CURLE_OK)
        {
          fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
          goto stop;
        }

      /* verifying the HTTP status code */
      curlCheckHttpHeader(&headerBuf, &http_status, &http_code);
      if (http_status != 200)
        {
          fprintf(stderr, "Invalid HTTP status code: %d %s\n",
                  http_status, http_code);
          if (http_code != NULL)
            free(http_code);
          goto stop;
        }
      if (http_code != NULL)
        free(http_code);

      curlMemBufferReset(&headerBuf);
      *data_len = bodyBuf.WriteOffset;
      *data = (unsigned char *) malloc(*data_len);
      memcpy(*data, (const unsigned char *) (bodyBuf.Buffer), *data_len);
      retcode = true;

    stop:
      curlMemBufferReset(&headerBuf);
      curlMemBufferReset(&bodyBuf);
      curl_easy_cleanup(curl);
    }

  return retcode;
}

#ifdef CURL_URL_GET             // only if recent Curl support is available

static void
do_check_query_url(const char *query, int *add_service, int *add_version,
                   int *add_request)
{
//
// checking if URL arguments SERVICE=, VERSION= or REQUEST= are alredy set
//
  int max = 64;
  char arg_name[64];
  char *p_out = arg_name;
  const char *p_in = query;
  int ignore = 0;

  *add_service = 1;
  *add_version = 1;
  *add_request = 1;

// it there are no args defined we surely need to add them
  if (query == NULL)
    return;

  while (1)
    {
      // parsing all URL arguments
      if (*p_in == '&')
        {
          p_in++;
          ignore = 0;
          continue;
        }
      if (*p_in == '=')
        {
          *p_out = '\0';
          if (strcasecmp(arg_name, "service") == 0)
            *add_service = 0;
          if (strcasecmp(arg_name, "version") == 0)
            *add_version = 0;
          if (strcasecmp(arg_name, "request") == 0)
            *add_request = 0;
          p_out = arg_name;
          p_in++;
          ignore = 1;
          continue;
        }
      if (*p_in == '\0')
        {
          *p_out = '\0';
          if (strcasecmp(arg_name, "service") == 0)
            *add_service = 0;
          if (strcasecmp(arg_name, "version") == 0)
            *add_version = 0;
          if (strcasecmp(arg_name, "request") == 0)
            *add_request = 0;
          break;
        }
      if (ignore)
        p_in++;
      else
        {
          if (p_out - arg_name < max - 1)
            *p_out++ = *p_in++;
          else
            p_in++;
        }
    }
}

static char *do_normalize_url(CURLU * h_in, const char *url_in)
{
//
// attempting to rewrite a normalized URL supporting
// SERVICE=WMS&VERSION=9.9.9&REQUEST=GetCapabilities
//
  char *url_out = NULL;
  char *host = NULL;
  char *scheme = NULL;
  char *user = NULL;
  char *password = NULL;
  char *port = NULL;
  char *path = NULL;
  char *query = NULL;
  char *fragment = NULL;
  int ok_host = 0;
  int ok_scheme = 0;
  int ok_user = 0;
  int ok_password = 0;
  int ok_port = 0;
  int ok_path = 0;
  int ok_query = 0;
  int ok_fragment = 0;
  int add_service = 1;
  int add_version = 1;
  int add_request = 1;
  int retval = 1;
  CURLU *h_out;
  CURLUcode uc;

// binding the input URL to its handle
  uc = curl_url_set(h_in, CURLUPART_URL, url_in, 0);
  if (uc)
    return NULL;

// breaking the input URL in its elementary elements
  uc = curl_url_get(h_in, CURLUPART_HOST, &host, 0);
  if (!uc)
    ok_host = 1;
  uc = curl_url_get(h_in, CURLUPART_SCHEME, &scheme, 0);
  if (!uc)
    ok_scheme = 1;
  uc = curl_url_get(h_in, CURLUPART_USER, &user, 0);
  if (!uc)
    ok_user = 1;
  uc = curl_url_get(h_in, CURLUPART_PASSWORD, &password, 0);
  if (!uc)
    ok_password = 1;
  uc = curl_url_get(h_in, CURLUPART_PORT, &port, 0);
  if (!uc)
    ok_port = 1;
  uc = curl_url_get(h_in, CURLUPART_PATH, &path, 0);
  if (!uc)
    ok_path = 1;
  uc = curl_url_get(h_in, CURLUPART_QUERY, &query, 0);
  if (!uc)
    ok_query = 1;
  uc = curl_url_get(h_in, CURLUPART_FRAGMENT, &fragment, 0);
  if (!uc)
    ok_fragment = 1;

// creating a Curl handle supporting the output URL
  h_out = curl_url();
  if (!h_out)
    {
      retval = 0;
      goto cleanup;
    }
// checking if SERVICE=, VERSION= and REQUEST= are actually defined
  do_check_query_url(query, &add_service, &add_version, &add_request);

// rewriting the output URL
  if (ok_host)
    {
      uc = curl_url_set(h_out, CURLUPART_HOST, host, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_scheme)
    {
      uc = curl_url_set(h_out, CURLUPART_SCHEME, scheme, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_user)
    {
      uc = curl_url_set(h_out, CURLUPART_USER, user, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_password)
    {
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_port)
    {
      uc = curl_url_set(h_out, CURLUPART_PORT, port, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_path)
    {
      uc = curl_url_set(h_out, CURLUPART_PATH, path, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_query)
    {
      uc = curl_url_set(h_out, CURLUPART_QUERY, query, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (ok_fragment)
    {
      uc = curl_url_set(h_out, CURLUPART_FRAGMENT, fragment, 0);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }

  if (add_service)
    {
      uc =
        curl_url_set(h_out, CURLUPART_QUERY, "SERVICE=WMS", CURLU_APPENDQUERY);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (add_version)
    {
      uc =
        curl_url_set(h_out, CURLUPART_QUERY, "VERSION=9.9.9",
                     CURLU_APPENDQUERY);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
  if (add_request)
    {
      uc =
        curl_url_set(h_out, CURLUPART_QUERY, "REQUEST=GetCapabilities",
                     CURLU_APPENDQUERY);
      if (uc)
        {
          retval = 0;
          goto cleanup;
        }
    }
// extracting the normalized output URL
  uc = curl_url_get(h_out, CURLUPART_URL, &url_out, 0);
  if (!uc)
    retval = 1;
  else
    retval = 0;
  curl_url_cleanup(h_out);

cleanup:
  if (host != NULL)
    curl_free(host);
  if (scheme != NULL)
    curl_free(scheme);
  if (user != NULL)
    curl_free(user);
  if (password != NULL)
    curl_free(password);
  if (port != NULL)
    curl_free(port);
  if (path != NULL)
    curl_free(path);
  if (query != NULL)
    curl_free(query);
  if (fragment != NULL)
    curl_free(fragment);
  if (!retval)
    {
      if (url_out != NULL)
        curl_free(url_out);
      url_out = NULL;
    }
  return url_out;
}

#endif // end Curl conditional

char *WmsDialog::NormalizeUrl(char const *url)
{
//
// attempting to normalize/complete a WMS GetCapabilities URL
//
#ifdef CURL_URL_GET             // only if recent Curl support is available

  char *norm_url;
  CURLU *h = curl_url();        // get a handle to the input URL
  if (!h)
    return NULL;
  norm_url = do_normalize_url(h, url);
  curl_url_cleanup(h);
  return norm_url;

#else
  return NULL;
#endif
}

void WmsDialog::DestroyNormalizedUrl(char *url)
{
//
// memory cleanup
//
  if (url != NULL)
    curl_free(url);
}
