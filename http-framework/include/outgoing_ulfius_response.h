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

#ifndef OUTGOING_ULFIUS_RESPONSE_H
#define OUTGOING_ULFIUS_RESPONSE_H

#include "response.h"

struct COutgoingUlfiusResponse;
typedef struct COutgoingUlfiusResponse COutgoingUlfiusResponse;
COutgoingUlfiusResponse *new_OutgoingUlfiusResponse(struct _u_response *u_response);
void delete_OutgoingUlfiusResponse(COutgoingUlfiusResponse *c_response);
void OutgoingUlfiusResponse_setBody(COutgoingUlfiusResponse *c_response, uint8_t *c_binary_data, size_t size);
void OutgoingUlfiusResponse_setCode(COutgoingUlfiusResponse *c_response, const CStatusCode c_code);
void OutgoingUlfiusResponse_setHeader(COutgoingUlfiusResponse *c_response,
                                      const char *c_header, const char *c_value);

#endif // OUTGOING_ULFIUS_RESPONSE_H
