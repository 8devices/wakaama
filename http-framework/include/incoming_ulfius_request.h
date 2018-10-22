/*
 * MIT License
 *
 * Copyright (c) 2018 8devices
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

#ifndef INCOMING_ULFIUS_REQUEST_H
#define INCOMING_ULFIUS_REQUEST_H

#include "request.h"

struct CIncomingUlfiusRequest;
typedef struct CIncomingUlfiusRequest CIncomingUlfiusRequest;
CIncomingUlfiusRequest *new_IncomingUlfiusRequest(const struct _u_request *u_request);
void delete_IncomingUlfiusRequest(CIncomingUlfiusRequest *c_request);
char *IncomingUlfiusRequest_getPath(CIncomingUlfiusRequest *c_request);
char *IncomingUlfiusRequest_getMethod(CIncomingUlfiusRequest *c_request);
char *IncomingUlfiusRequest_getHeader(CIncomingUlfiusRequest *c_request, const char *c_header);
uint8_t *IncomingUlfiusRequest_getBody(CIncomingUlfiusRequest *c_request);

#endif // INCOMING_ULFIUS_REQUEST_H
