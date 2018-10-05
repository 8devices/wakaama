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


#include "http_framework.h"

#ifdef __cplusplus
extern "C"{
#endif

#include <ulfius.h>

#ifdef __cplusplus
}
#endif

typedef int (*ulfius_callback_function_t)(const struct _u_request *, struct _u_response *, void *);

std::map<std::string, std::string> ulfiusToStdMap(struct _u_map *ulfius_map);

class IncomingUlfiusRequest: public Request
{
public:
    IncomingUlfiusRequest (const struct _u_request *u_request);
    std::string getPath() const;
    std::string getMethod() const;
    std::string getHeader(const std::string header);
    std::vector<uint8_t> getBody() const;
};

class OutgoingUlfiusResponse: public Response
{
public:
    OutgoingUlfiusResponse(struct _u_response *u_response);
    void setBody(std::vector<uint8_t> binary_data);
    void setCode(StatusCode code);
    void setHeader(const std::string header, const std::string value);

private:
    struct _u_response *ulfius_response;
};

class CallbackHandler
{
public:
    CallbackHandler(callback_function_t handler_function, void *handler_context);
    callback_function_t function;
    void *context;
};

class UlfiusHttpFramework: public HttpFramework
{
public:
    UlfiusHttpFramework(int port);
    ~UlfiusHttpFramework();

    void startFramework();
    void startSecureFramework(char *private_key_file, char *certificate_file);
    void stopFramework();

    void addHandler(
        const std::string method, const std::string url_prefix,
        unsigned int priority, callback_function_t handler_function, void *handler_context);

private:
    static int ulfiusCallback(const struct _u_request *u_request,
                              struct _u_response *u_response, void *context);

    struct _u_instance ulfius_instance;
};
