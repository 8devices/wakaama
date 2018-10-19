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

#include "../include/ulfius_http_framework.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/ulfius_http_framework.h"

CIncomingUlfiusRequest *new_IncomingUlfiusRequest(const struct _u_request *u_request)
{
    return reinterpret_cast<CIncomingUlfiusRequest *>(new IncomingUlfiusRequest(u_request));
}
void delete_IncomingUlfiusRequest(CIncomingUlfiusRequest *c_request)
{
    delete reinterpret_cast<IncomingUlfiusRequest *>(c_request);
}
char *IncomingUlfiusRequest_getPath(CIncomingUlfiusRequest *c_request)
{
    IncomingUlfiusRequest *request = reinterpret_cast<IncomingUlfiusRequest*>(c_request);
    std::string path = request->getPath();

    return const_cast<char *>(path.c_str());
}
char *IncomingUlfiusRequest_getMethod(CIncomingUlfiusRequest *c_request)
{
    IncomingUlfiusRequest *request = reinterpret_cast<IncomingUlfiusRequest*>(c_request);
    std::string method = request->getMethod();

    return const_cast<char *>(method.c_str());
}
char *IncomingUlfiusRequest_getHeader(CIncomingUlfiusRequest *c_request, const char *c_header)
{
    IncomingUlfiusRequest *request = reinterpret_cast<IncomingUlfiusRequest*>(c_request);
    const std::string header(c_header);
    std::string header_value = request->getHeader(header);

    return const_cast<char *>(header_value.c_str());
}
uint8_t *IncomingUlfiusRequest_getBody(CIncomingUlfiusRequest *c_request)
{
    // XXX: note that body should be freed after use!
    IncomingUlfiusRequest *request = reinterpret_cast<IncomingUlfiusRequest*>(c_request);
    std::vector<uint8_t> body = request->getBody();
    uint8_t *c_body = static_cast<uint8_t *>(malloc(sizeof(uint8_t) * body.size()));

    std::copy(body.begin(), body.end(), c_body);

    return c_body;
}

COutgoingUlfiusResponse *new_OutgoingUlfiusResponse(struct _u_response *u_response)
{
    return reinterpret_cast<COutgoingUlfiusResponse*>(new OutgoingUlfiusResponse(u_response));
}
void delete_OutgoingUlfiusResponse(COutgoingUlfiusResponse *c_response)
{
    delete reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
}
void OutgoingUlfiusResponse_setBody(COutgoingUlfiusResponse *c_response, uint8_t *binary_data, size_t size)
{
    OutgoingUlfiusResponse *response = reinterpret_cast<OutgoingUlfiusResponse*>(c_response);
    std::vector<uint8_t> body(size);

    body.insert(body.end(), binary_data, binary_data + size);
    response->setBody(body);
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

CUlfiusHttpFramework *new_UlfiusHttpFramework(struct _u_instance *instance)
{
    return reinterpret_cast<CUlfiusHttpFramework*>(new UlfiusHttpFramework(instance));
}
void delete_UlfiusHttpFramework(CUlfiusHttpFramework *c_framework)
{
    delete reinterpret_cast<UlfiusHttpFramework*>(c_framework);
}
void UlfiusHttpFramework_startFramework(CUlfiusHttpFramework *c_framework)
{
    UlfiusHttpFramework *framework = reinterpret_cast<UlfiusHttpFramework*>(c_framework);
    framework->startFramework();
}
void UlfiusHttpFramework_startSecureFramework(CUlfiusHttpFramework *c_framework, const char *c_private_key_file, const char *c_certificate_file)
{
    UlfiusHttpFramework *framework = reinterpret_cast<UlfiusHttpFramework*>(c_framework);
    const std::string private_key_file(c_private_key_file);
    const std::string certificate_file(c_certificate_file);

    framework->startSecureFramework(private_key_file, certificate_file);
}
void UlfiusHttpFramework_stopFramework(CUlfiusHttpFramework *c_framework)
{
    UlfiusHttpFramework *framework = reinterpret_cast<UlfiusHttpFramework*>(c_framework);

    framework->stopFramework();
}
void UlfiusHttpFramework_addHandler(CUlfiusHttpFramework *c_framework,
    const char *method, const char *url_prefix, unsigned int priority,
    c_callback_function_t c_handler_function, void *handler_context)
{
    UlfiusHttpFramework *framework = reinterpret_cast<UlfiusHttpFramework*>(c_framework);
    callback_function_t handler_function = reinterpret_cast<callback_function_t>(c_handler_function);

    framework->addHandler(method, url_prefix, priority, handler_function, handler_context);
}

#ifdef __cplusplus
} // extern "C"
#endif

std::map<std::string, std::string> ulfiusToStdMap(struct _u_map *ulfius_map)
{
    int header_iterator;
    std::map<std::string, std::string> std_map;
    std::string key, value;

    if (ulfius_map == NULL)
    {
        return std_map;
    }

    for (header_iterator=0; ulfius_map->keys[header_iterator] != NULL; header_iterator++) {
        key = ulfius_map->keys[header_iterator];
        if (ulfius_map->lengths[header_iterator] > 0) {
            value = ulfius_map->values[header_iterator];
        } else {
            value = "";
        }
        std_map.insert(std::make_pair(key, value));
    }

    return std_map;
}

IncomingUlfiusRequest::IncomingUlfiusRequest(const struct _u_request *u_request)
{
    uint8_t *uint_body = (uint8_t*)u_request->binary_body;
    std::vector<uint8_t> vector_body(uint_body, uint_body + u_request->binary_body_length);
    std::string tmp_path(u_request->http_url);
    std::string tmp_method(u_request->http_verb);

    path = tmp_path;
    method = tmp_method;
    headers = ulfiusToStdMap(u_request->map_header);
    body = vector_body;
}
IncomingUlfiusRequest::~IncomingUlfiusRequest() { }
std::string IncomingUlfiusRequest::getPath()
{
    return path;
}
std::string IncomingUlfiusRequest::getMethod()
{
     return method;
}
std::string IncomingUlfiusRequest::getHeader(const std::string header)
{
    std::string header_value;
    std::map<std::string, std::string>::iterator headers_iterator;

    headers_iterator = headers.find(header);

    if (headers_iterator != headers.end())
    {
        header_value = headers_iterator->second;
    }
    return header_value;
}
std::vector<uint8_t> IncomingUlfiusRequest::getBody()
{
    return body;
}

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

    ulfius_response->binary_body = malloc(body.size());
    std::memcpy(ulfius_response->binary_body, &body, body.size());
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

CallbackHandler::CallbackHandler(callback_function_t handler_function, void *handler_context)
{
    function = handler_function;
    context = handler_context;
}

UlfiusHttpFramework::UlfiusHttpFramework(struct _u_instance *instance)
{
    ulfius_instance = instance; 
}
UlfiusHttpFramework::~UlfiusHttpFramework()
{
    for(std::vector<CallbackHandler *>::size_type index = 0;
        index < callbackHandlers.size(); ++index)
    {
        delete callbackHandlers[index];
    }
}
void UlfiusHttpFramework::startFramework()
{
    ulfius_start_framework(ulfius_instance);
}
void UlfiusHttpFramework::startSecureFramework(const std::string private_key_file,
                                               const std::string certificate_file)
{
    ulfius_start_secure_framework(ulfius_instance, private_key_file.c_str(),
                                  certificate_file.c_str());
}
void UlfiusHttpFramework::stopFramework()
{
    ulfius_stop_framework(ulfius_instance);
}
int UlfiusHttpFramework::ulfiusCallback(const struct _u_request *u_request,
                                        struct _u_response *u_response, void *context)
{
    IncomingUlfiusRequest *request = new IncomingUlfiusRequest(u_request);
    StatusCode callback_status_code;

    OutgoingUlfiusResponse *response = new OutgoingUlfiusResponse(u_response);

    CallbackHandler *handler = reinterpret_cast<CallbackHandler *>(context);
    callback_status_code = handler->function(request, response, handler->context);

    delete request;
    delete response;

    switch (callback_status_code)
    {
    case information_continue:
        return U_CALLBACK_CONTINUE;

    case success_ok:
    case success_created:
    case success_accepted:
    case success_no_content:
    case success_reset_content:
        return U_CALLBACK_COMPLETE;

    case client_error_unauthorized:
        return U_CALLBACK_UNAUTHORIZED;

    case client_error:
    case client_error_forbidden:
    case client_error_not_found:
    case client_error_method_not_allowed:
    case client_error_not_acceptable:
    case server_error_internal_server_error:
        return U_CALLBACK_ERROR;

    default:
        return U_CALLBACK_ERROR;
    }
}

void UlfiusHttpFramework::addHandler(
    const std::string method, const std::string url_prefix,
    unsigned int priority, callback_function_t handler_function, void *handler_context)
{
    CallbackHandler *handler = new CallbackHandler(handler_function, handler_context);
    callbackHandlers.push_back(handler);
    ulfius_add_endpoint_by_val(ulfius_instance, method.c_str(),
                               url_prefix.c_str(), NULL, priority, &ulfiusCallback,
                               reinterpret_cast<void *>(&handler));
}
