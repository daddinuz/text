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

#include <stddef.h>
#include <stdbool.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

#define TEXT_VERSION_MAJOR        0
#define TEXT_VERSION_MINOR        1
#define TEXT_VERSION_PATCH        0
#define TEXT_VERSION_SUFFIX       ""
#define TEXT_VERSION_IS_RELEASE   0
#define TEXT_VERSION_HEX          0x000100

/**
 * A string library with support for complex operations on textual data
 *
 * @attention Every function in this library terminates the program in case of out of memory.
 */
typedef char *Text;

/**
 * Creates an empty text using default capacity.
 *
 * @return A new text instance.
 */
extern Text Text_new(void)
__attribute__((__warn_unused_result__));

/**
 * Creates a new text with at least the given initial capacity.
 *
 * @attention capacity must be less than SIZE_MAX.
 *
 * @param capacity The initial capacity.
 * @return A new text instance.
 */
extern Text Text_withCapacity(size_t capacity)
__attribute__((__warn_unused_result__));

/**
 * Creates a new text from the given bytes array.
 *
 * @attention bytes array must not be NULL.
 * @attention the size of the bytes array must be less than SIZE_MAX.
 *
 * @param bytes The bytes array.
 * @param size The size of the array.
 * @return A new text instance.
 */
extern Text Text_fromBytes(const void *bytes, size_t size)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Creates a new text from the given literal.
 *
 * @attention literal must not be NULL.
 * @attention the length of the literal must be less than SIZE_MAX.
 *
 * @param literal The string literal.
 * @return A new text instance.
 */
extern Text Text_fromLiteral(const char *literal)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Overwrites the content of the text (expanding it's capacity if needed) with the given bytes array.
 *
 * @attention ref and *ref must not be NULL.
 * @attention bytes must not be NULL.
 * @attention size must be less than SIZE_MAX.
 *
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @param ref The text instance reference.
 * @param bytes The bytes array to write in the text.
 * @param size The size of the bytes array.
 * @return the modified text instance
 */
extern Text Text_overwriteWithBytes(Text *ref, const void *bytes, size_t size)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Overwrites the content of the text (expanding it's capacity if needed) with the given literal.
 *
 * @attention ref and *ref must not be NULL.
 * @attention literal must not be NULL.
 * @attention the length of the literal must be less than SIZE_MAX.
 *
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @param ref The text instance reference.
 * @param literal The literal to write in the text.
 * @return the modified text instance
 */
extern Text Text_overwriteWithLiteral(Text *ref, const char *literal)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Appends the bytes array to the text.
 *
 * @attention ref and *ref must not be NULL.
 * @attention bytes must not be NULL.
 * @attention size must be less than SIZE_MAX.
 *
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @param ref The text instance reference.
 * @param bytes The bytes array to append.
 * @param size The size of the bytes array.
 * @return the modified text instance
 */
extern Text Text_appendBytes(Text *ref, const void *bytes, size_t size)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Appends the literal to the text.
 *
 * @attention ref and *ref must not be NULL.
 * @attention literal must not be NULL.
 * @attention the length of the literal must be less than SIZE_MAX.
 *
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @param ref The text instance reference.
 * @param literal The literal to append.
 * @return the modified text instance
 */
extern Text Text_appendLiteral(Text *ref, const char *literal)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Clears the content of the text without changing its capacity.
 *
 * @attention self must not be NULL.
 *
 * @param self The text instance.
 */
extern void Text_clear(Text self)
__attribute__((__nonnull__));

/**
 * Expands (if needed) the text to fit the requested capacity.
 *
 * @attention ref and *ref must not be NULL.
 * @attention capacity must be less than SIZE_MAX.
 *
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @return the modified text instance
 */
extern Text Text_expandToFit(Text *ref, size_t capacity)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Clears the capacity of the text to fit the size of its content.
 *
 * @attention ref and *ref must not be NULL.
 * @attention the reference to the text will be invalidated after this call, the new text is returned.
 *
 * @return the modified text instance
 */
extern Text Text_shrink(Text *ref)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Gets the character in the text at the given index.
 *
 * @attention self must not be NULL.
 * @attention terminates execution if index is greater or equals the text's length.
 */
extern char Text_get(Text self, size_t index)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Puts the character in the text at the given index replacing the existing one.
 *
 * @attention self must not be NULL.
 * @attention terminates execution if index is greater or equals the text's length.
 *
 * @return the replaced character.
 */
extern char Text_put(Text self, size_t index, char c)
__attribute__((__nonnull__));

/**
 * Gets the size of the text.
 *
 * @attention self must not be NULL.
 *
 * @param self The text instance.
 * @return The size of the text.
 */
extern size_t Text_length(Text self)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Gets the capacity of the text.
 *
 * @attention self must not be NULL.
 *
 * @param self The text instance.
 * @return The capacity of the text.
 */
extern size_t Text_capacity(Text self)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Checks for equality.
 *
 * @attention self must not be NULL.
 * @attention other must not be NULL.
 */
extern bool Text_equals(Text self, Text other)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Deletes an instance of a text.
 * If NULL nothing will be done.
 *
 * @param self The instance to be deleted.
 */
extern void Text_delete(Text self);

#ifdef __cplusplus
}
#endif
