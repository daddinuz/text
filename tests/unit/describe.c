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

#include "features.h"

Describe("Text",
         Trait("constructors",
               Run(new),
               Run(withCapacity),
               Run(withCapacity_checkRuntimeErrors),
               Run(quoted),
               Run(quoted_checkRuntimeErrors),
               Run(format),
               Run(format_checkRuntimeErrors),
               Todo(vFormat),
               Todo(vFormat_checkRuntimeErrors),
               Run(fromBytes),
               Run(fromBytes_checkRuntimeErrors),
               Run(fromLiteral),
               Run(fromLiteral_checkRuntimeErrors)),
         Trait("modify content",
               Run(overwriteWithBytes),
               Run(overwriteWithBytes_checkRuntimeErrors),
               Run(overwriteWithLiteral),
               Run(overwriteWithLiteral_checkRuntimeErrors),
               Run(appendFormat),
               Run(appendFormat_checkRuntimeErrors),
               Todo(vAppendFormat),
               Todo(vAppendFormat_checkRuntimeErrors),
               Run(appendBytes),
               Run(appendBytes_checkRuntimeErrors),
               Run(appendLiteral),
               Run(appendLiteral_checkRuntimeErrors),
               Run(clear),
               Run(clear_checkRuntimeErrors)),
         Trait("modify capacity",
               Run(expandToFit),
               Run(expandToFit_checkRuntimeErrors),
               Run(shrink),
               Run(shrink_checkRuntimeErrors)),
         Trait("accessors",
               Run(get),
               Run(get_checkRuntimeErrors),
               Run(put),
               Run(put_checkRuntimeErrors)),
         Trait("getters",
               Run(length_checkRuntimeErrors),
               Run(capacity_checkRuntimeErrors)),
         Trait("equality",
               Run(equals),
               Run(equals_checkRuntimeErrors)))
