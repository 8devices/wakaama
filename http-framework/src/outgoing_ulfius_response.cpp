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

#include <cstring>

#include "../include/outgoing_ulfius_response.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <ulfius.h>

#include "../include/outgoing_ulfius_response.h"

COutgoingUlfiusResponse *new_OutgoingUlfiusResponse(struct _u_response *u_response)
{
    return reinterpret_cast<COutgoingUlfiusResponse*>(new OutgoingUlfiusResponse(u_response));
}
void delete_OutgoingUlfiusResponse(COutgoingUlfiusResponse *c_response)
{
    delete reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
}
void OutgoingUlfiusResponse_setBody(COutgoingUlfiusResponse *c_response, uint8_t *c_binary_data, size_t size)
{
    OutgoingUlfiusResponse *response = reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
    std::vector<uint8_t> binary_data(size);

    binary_data.insert(binary_data.end(), c_binary_data, c_binary_data + size);
    response->setBody(binary_data);
}
void OutgoingUlfiusResponse_setCode(COutgoingUlfiusResponse *c_response,
                                    const CStatusCode c_code)
{
    OutgoingUlfiusResponse *response = reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
    const StatusCode code = static_cast<StatusCode>(c_code);
    response->setCode(code);
}
void OutgoingUlfiusResponse_setHeader(COutgoingUlfiusResponse *c_response, const char *c_header, const char *c_value)
{
    OutgoingUlfiusResponse *response = reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
    const std::string header(c_header);
    const std::string value(c_value);

    response->setHeader(header, value);
}

#ifdef __cplusplus
} // extern "C"
#endif

OutgoingUlfiusResponse::OutgoingUlfiusResponse(struct _u_response *u_response)
{
    ulfius_response = u_response;
}
OutgoingUlfiusResponse::~OutgoingUlfiusResponse() { }
void OutgoingUlfiusResponse::setBody(std::vector<uint8_t> binary_data)
{
    if (ulfius_response->binary_body != NULL)
    {
         free(ulfius_response->binary_body);
    }

    ulfius_response->binary_body = malloc(binary_data.size());
    std::memcpy(ulfius_response->binary_body, &binary_data, binary_data.size());
}
void OutgoingUlfiusResponse::setCode(StatusCode code)
{
    ulfius_response->status = static_cast<int>(code);
}
void OutgoingUlfiusResponse::setHeader(const std::string header, const std::string value)
{
    const char *c_header = header.c_str();
    const char *c_value = value.c_str();

    u_map_put(ulfius_response->map_header, c_header, c_value);
}
