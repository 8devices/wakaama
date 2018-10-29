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

#ifndef ULFIUS_HTTP_FRAMEWORK_H
#define ULFIUS_HTTP_FRAMEWORK_H

#include "http_framework.h"

struct CUlfiusHttpFramework;
typedef struct CUlfiusHttpFramework CUlfiusHttpFramework;
CUlfiusHttpFramework *new_UlfiusHttpFramework(struct _u_instance *instance);
void delete_UlfiusHttpFramework(CUlfiusHttpFramework *c_framework);
void UlfiusHttpFramework_startFramework(CUlfiusHttpFramework *c_framework);
void UlfiusHttpFramework_startSecureFramework(
    CUlfiusHttpFramework *c_framework, const char *c_private_key_file,
    const char *c_certificate_file);
void UlfiusHttpFramework_stopFramework(CUlfiusHttpFramework *c_framework);
void UlfiusHttpFramework_addHandler(
    CUlfiusHttpFramework *c_framework,
    const char *method, const char *url_prefix,
    unsigned int priority, c_callback_function_t handler_function, void *handler_context);

#endif // ULFIUS_HTTP_FRAMEWORK_H
