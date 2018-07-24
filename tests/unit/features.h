/*
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 *
 * Copyright (c) 2018 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <traits-unit/traits-unit.h>

Feature(new);

Feature(withCapacity);
Feature(withCapacity_checkRuntimeErrors);

Feature(format);
Feature(format_checkRuntimeErrors);

Feature(fromBytes);
Feature(fromBytes_checkRuntimeErrors);

Feature(fromLiteral);
Feature(fromLiteral_checkRuntimeErrors);

Feature(overwriteWithBytes);
Feature(overwriteWithBytes_checkRuntimeErrors);

Feature(overwriteWithLiteral);
Feature(overwriteWithLiteral_checkRuntimeErrors);

Feature(appendBytes);
Feature(appendBytes_checkRuntimeErrors);

Feature(appendLiteral);
Feature(appendLiteral_checkRuntimeErrors);

Feature(clear);
Feature(clear_checkRuntimeErrors);

Feature(expandToFit);
Feature(expandToFit_checkRuntimeErrors);

Feature(shrink);
Feature(shrink_checkRuntimeErrors);

Feature(get);
Feature(get_checkRuntimeErrors);

Feature(put);
Feature(put_checkRuntimeErrors);

Feature(length_checkRuntimeErrors);
Feature(capacity_checkRuntimeErrors);

Feature(equals);
Feature(equals_checkRuntimeErrors);

#ifdef __cplusplus
}
#endif
