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

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <panic/panic.h>
#include <alligator/alligator.h>
#include "text.h"
#include "text_config.h"

#if TEXT_DEFAULT_CAPACITY < 32UL || TEXT_DEFAULT_CAPACITY >= SIZE_MAX
    #error
#endif

static size_t nextEven(const size_t size) {
    assert(size < SIZE_MAX);
    return size + (size % 2);
}

static size_t applyLoadFactor(const size_t size) {
    assert(size < SIZE_MAX);
    return nextEven((size_t) (size * TEXT_LOAD_FACTOR));
}

static size_t calculateNewCapacity(size_t currentCapacity, const size_t targetCapacity) {
    assert(currentCapacity < SIZE_MAX);
    assert(targetCapacity < SIZE_MAX);
    assert(currentCapacity < targetCapacity);
    while (currentCapacity < targetCapacity) {
        currentCapacity = applyLoadFactor(currentCapacity);
    }
    return currentCapacity;
}

struct Text_Header {
    size_t capacity;
    size_t length;
    char *content;
};

Text Text_new(void) {
    return Text_withCapacity(TEXT_DEFAULT_CAPACITY);
}

Text Text_withCapacity(size_t capacity) {
    assert(capacity < SIZE_MAX);
    if (capacity < TEXT_DEFAULT_CAPACITY) {
        capacity = TEXT_DEFAULT_CAPACITY;
    }
    struct Text_Header *header = Option_unwrap(Alligator_malloc(
            sizeof(*header) + sizeof(header->content[0]) * (capacity + 1)
    ));
    header->content = (char *) (header + 1);
    header->content[header->length = 0] = 0;
    header->content[header->capacity = capacity] = 0;
    return header->content;
}

Text Text_fromBytes(const void *const bytes, const size_t size) {
    assert(bytes);
    assert(size < SIZE_MAX);
    Text text = Text_withCapacity(size);
    struct Text_Header *header = (struct Text_Header *) text - 1;
    memcpy(text, bytes, size);
    header->length = size;
    text[size] = 0;
    return text;
}

Text Text_fromLiteral(const char *const literal) {
    assert(literal);
    return Text_fromBytes(literal, strlen(literal));
}

Text Text_overwriteWithBytes(Text *ref, const void *const bytes, const size_t size) {
    assert(ref);
    assert(*ref);
    assert(bytes);
    assert(size < SIZE_MAX);
    Text text = Text_expandToFit(ref, size);
    struct Text_Header *header = (struct Text_Header *) text - 1;
    memmove(text, bytes, size);
    text[header->length = size] = 0;
    *ref = NULL;
    return text;
}

Text Text_overwriteWithLiteral(Text *ref, const char *const literal) {
    assert(ref);
    assert(*ref);
    assert(literal);
    return Text_overwriteWithBytes(ref, literal, strlen(literal));
}

Text Text_appendBytes(Text *ref, const void *const bytes, const size_t size) {
    assert(ref);
    assert(*ref);
    assert(bytes);
    assert(size);
    struct Text_Header *header = (struct Text_Header *) (*ref) - 1;
    const size_t currentSize = header->length, newSize = size + currentSize;
    Text text = Text_expandToFit(ref, newSize);
    header = (struct Text_Header *) text - 1;
    memmove(text + currentSize, bytes, size);
    text[header->length = newSize] = 0;
    *ref = NULL;
    return text;
}

Text Text_appendLiteral(Text *ref, const char *const literal) {
    assert(ref);
    assert(*ref);
    assert(literal);
    return Text_appendBytes(ref, literal, strlen(literal));
}

void Text_clear(Text self) {
    assert(self);
    struct Text_Header *header = (struct Text_Header *) self - 1;
    self[header->length = 0] = 0;
}

Text Text_expandToFit(Text *ref, size_t capacity) {
    assert(ref);
    assert(*ref);
    assert(capacity < SIZE_MAX);
    struct Text_Header *header = (struct Text_Header *) (*ref) - 1;
    if (capacity > header->capacity) {
        capacity = calculateNewCapacity(header->capacity, capacity);
        header = Option_unwrap(Alligator_realloc(
                header, sizeof(*header) + sizeof(header->content[0]) * (capacity + 1)
        ));
        header->content = (char *) (header + 1);
        header->content[header->capacity = capacity] = 0;
    }
    *ref = NULL;
    return header->content;
}

Text Text_shrink(Text *ref) {
    assert(ref);
    assert(*ref);
    struct Text_Header *header = (struct Text_Header *) (*ref) - 1;
    const size_t size = header->length;
    if (size < header->capacity) {
        header = Option_unwrap(Alligator_realloc(
                header, sizeof(*header) + sizeof(header->content[0]) * (size + 1)
        ));
        header->content = (char *) (header + 1);
        header->content[header->capacity = size] = 0;
    }
    *ref = NULL;
    return header->content;
}

char Text_get(Text self, const size_t index) {
    assert(self);
    if (index >= Text_length(self)) {
        Panic_terminate("Out of range");
    }
    return self[index];
}

char Text_put(Text self, const size_t index, const char c) {
    assert(self);
    if (index >= Text_length(self)) {
        Panic_terminate("Out of range");
    }
    char *p = self + index;
    const char bk = *p;
    *p = c;
    return bk;
}

size_t Text_length(Text self) {
    assert(self);
    struct Text_Header *header = (struct Text_Header *) self - 1;
    return header->length;
}

size_t Text_capacity(Text self) {
    assert(self);
    struct Text_Header *header = (struct Text_Header *) self - 1;
    return header->capacity;
}

bool Text_equals(Text self, Text other) {
    assert(self);
    assert(other);
    const size_t length = Text_length(self);
    return length == Text_length(other) && 0 == memcmp(self, other, length);
}

void Text_delete(Text self) {
    if (self) {
        struct Text_Header *header = (struct Text_Header *) self - 1;
        Alligator_free(header);
    }
}
