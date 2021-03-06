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

#include <stdint.h>
#include <text.h>
#include <text_config.h>
#include <traits/traits.h>
#include "features.h"

struct ByteArray {
    const char *bytes;
    const size_t size;
};

Feature(new) {
    Text sut = Text_new();

    assert_equal(TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
    assert_equal(0, Text_length(sut));
    assert_string_equal("", sut);

    Text_delete(sut);
}

Feature(withCapacity) {
    Text sut = NULL;
    const size_t capacities[] = {
            0,
            TEXT_DEFAULT_CAPACITY / 2, TEXT_DEFAULT_CAPACITY - 1,
            TEXT_DEFAULT_CAPACITY,
            TEXT_DEFAULT_CAPACITY + 1, TEXT_DEFAULT_CAPACITY * 2
    };
    const size_t capacitiesSize = sizeof(capacities) / sizeof(capacities[0]);

    for (size_t i = 0; i < capacitiesSize; i++) {
        const size_t capacity = capacities[i];

        Text_delete(sut);
        sut = Text_withCapacity(capacity);

        if (capacity <= TEXT_DEFAULT_CAPACITY) {
            assert_equal(TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        } else {
            assert_equal(capacity, Text_capacity(sut));
        }

        assert_equal(0, Text_length(sut));
        assert_string_equal("", sut);
    }

    Text_delete(sut);
}

Feature(withCapacity_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_withCapacity(SIZE_MAX);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(quoted) {
    Text sut = NULL;

    {
        const char BYTES[] = "";
        const char EXPECTED[] = "\"\"";
        const size_t BYTES_SIZE = 0;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_quoted(BYTES, BYTES_SIZE);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED, "%s", sut);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);
    }

    Text_delete(sut);

    {
        const char BYTES[] = " \" \\ / \b \f \n \r \t \0 ";
        const char EXPECTED[] = "\" \\\" \\\\ \\/ \\b \\f \\n \\r \\t \\u0000 \"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_quoted(BYTES, BYTES_SIZE);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);
    }

    Text_delete(sut);

    {
        const char BYTES[] = "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o";
        const char EXPECTED[] = "\"a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_quoted(BYTES, BYTES_SIZE);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);
    }

    Text_delete(sut);

    {
        const char BYTES[] = "1a\"2b\\3c/4d\b5e\f6f\n7g\r8h\t9i\0" "10l\1" "11m\2" "12n\3" "13o";
        const char EXPECTED[] = "\"1a\\\"2b\\\\3c\\/4d\\b5e\\f6f\\n7g\\r8h\\t9i\\u000010l\\u000111m\\u000212n\\u000313o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_quoted(BYTES, BYTES_SIZE);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);
    }

    Text_delete(sut);

    {
        const char BYTES[] = "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o";
        const char EXPECTED[] = "\"a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_greater(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = EXPECTED_SIZE;

        sut = Text_quoted(BYTES, BYTES_SIZE);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);
    }

    Text_delete(sut);
}

Feature(quoted_checkRuntimeErrors) {
    Text sut = NULL;
    const void *bytes = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_quoted(bytes, 0);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        sut = Text_quoted("", SIZE_MAX);
        (void) sut;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
}

Feature(format) {
    Text sut = NULL;

    {
#define FORMAT      "Hello World!"

        char EXPECTED[1024];
        snprintf(EXPECTED, sizeof(EXPECTED) - 1, FORMAT);
        const size_t EXPECTED_SIZE = strlen(EXPECTED);
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_format(FORMAT);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

#undef FORMAT
    }

    Text_delete(sut);

    {
#define FORMAT      "%s", ""

        char EXPECTED[1024];
        snprintf(EXPECTED, sizeof(EXPECTED) - 1, FORMAT);
        const size_t EXPECTED_SIZE = strlen(EXPECTED);
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_format(FORMAT);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

#undef FORMAT
    }

    Text_delete(sut);

    {
#define FORMAT      "%s%c%s!%c", "Hello", ' ', "World", '\n'

        char EXPECTED[1024];
        snprintf(EXPECTED, sizeof(EXPECTED) - 1, FORMAT);
        const size_t EXPECTED_SIZE = strlen(EXPECTED);
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_format(FORMAT);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

#undef FORMAT
    }

    Text_delete(sut);

    {
#define FORMAT      "%*s %20d %-15zu %.5f %Lf %p %c", 25, "AaA", -543532, 629870197LU, 98763.4865f, 98739847.4L, NULL, '!'

        char EXPECTED[1024];
        snprintf(EXPECTED, sizeof(EXPECTED) - 1, FORMAT);
        const size_t EXPECTED_SIZE = strlen(EXPECTED);
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        sut = Text_format(FORMAT);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

#undef FORMAT
    }

    Text_delete(sut);

    {
#define FORMAT      "%*s %20d %-15zu %.5f %Lf %p %0*i", 25, "AaA", -543532, 629870197LU, 98763.4865f, 98739847.4L, NULL, 35, 0

        char EXPECTED[1024];
        snprintf(EXPECTED, sizeof(EXPECTED) - 1, FORMAT);
        const size_t EXPECTED_SIZE = strlen(EXPECTED);
        assert_greater(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = EXPECTED_SIZE;

        sut = Text_format(FORMAT);
        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(EXPECTED_CAPACITY, Text_capacity(sut));

#undef FORMAT
    }

    Text_delete(sut);
}

Feature(format_checkRuntimeErrors) {
    Text sut = NULL;
    const char *format = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_format(format, NULL);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(vFormat) {

}

Feature(vFormat_checkRuntimeErrors) {

}

Feature(fromBytes) {
    Text sut = NULL;
    const struct ByteArray bytesArray[] = {
            {.bytes="", .size=0},
            {.bytes="Hello world!", .size=12},
            {.bytes="sit,\0amet,\0consectetur,\0adipisci", .size=32}
    };
    const size_t bytesArraySize = sizeof(bytesArray) / sizeof(bytesArray[0]);

    for (size_t i = 0; i < bytesArraySize; i++) {
        const void *bytes = bytesArray[i].bytes;
        const size_t size = bytesArray[i].size;

        Text_delete(sut);
        sut = Text_fromBytes(bytes, size);

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_memory_equal(size, bytes, sut);
    }

    Text_delete(sut);
}

Feature(fromBytes_checkRuntimeErrors) {
    Text sut = NULL;
    const void *bytes = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_fromBytes(bytes, 8);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        sut = Text_fromBytes(&bytes, SIZE_MAX);
        (void) sut;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
}

Feature(fromLiteral) {
    Text sut = NULL;
    const char *literals[] = {"", "Hello world!", "lorem ipsum dolor", "sit,\0amet,\0consectetur,\0adipisci"};
    const size_t literalsSize = sizeof(literals) / sizeof(literals[0]);

    for (size_t i = 0; i < literalsSize; i++) {
        const char *literal = literals[i];
        const size_t length = strlen(literal);

        Text_delete(sut);
        sut = Text_fromLiteral(literal);

        assert_equal(length > TEXT_DEFAULT_CAPACITY ? length : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(length, Text_length(sut));
        assert_string_equal(literal, sut, "\n`%s` `%s`\n", literal, sut);
    }

    Text_delete(sut);
}

Feature(fromLiteral_checkRuntimeErrors) {
    Text sut = NULL;
    const char *literal = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_fromLiteral(literal);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(duplicate) {
    {
        Text seed = Text_fromLiteral("");
        Text sut = Text_duplicate(seed);

        assert_equal(Text_length(sut), Text_length(seed));
        assert_equal(Text_capacity(sut), Text_capacity(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromLiteral("lorem ipsum dolor");
        Text sut = Text_duplicate(seed);

        assert_equal(Text_length(sut), Text_length(seed));
        assert_equal(Text_capacity(sut), Text_capacity(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromBytes("lorem\0ipsum\0dolor", 17);
        Text sut = Text_duplicate(seed);

        assert_equal(Text_length(sut), Text_length(seed));
        assert_equal(Text_capacity(sut), Text_capacity(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }
}

Feature(duplicate_checkRuntimeErrors) {
    Text seed = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text sut = Text_duplicate(seed);
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(overwrite) {
    {
        Text seed = Text_new(), tmp = Text_new();

        Text sut = Text_overwrite(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), Text_length(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_new(), tmp = Text_fromLiteral("lorem ipsum");

        Text sut = Text_overwrite(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), Text_length(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromLiteral("lorem ipsum"), tmp = Text_new();

        Text sut = Text_overwrite(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), Text_length(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromLiteral("lorem ipsum"), tmp = Text_fromLiteral("foo spam");

        Text sut = Text_overwrite(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), Text_length(seed));
        assert_string_equal(sut, seed);

        Text_delete(sut);
        Text_delete(seed);
    }

    // TODO test expansion
}

Feature(overwrite_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {
        Text sut = NULL, other = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_overwrite(&sut, other);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
        Text_delete(other);
    }

    {
        Text sut = Text_new(), other = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_overwrite(&sut, other);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(overwriteWithFormat) {
    {
        Text tmp = Text_new();

        Text sut = Text_overwriteWithFormat(&tmp, "%s", "");
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), 0);
        assert_string_equal(sut, "");

        Text_delete(sut);
    }

    {
        Text tmp = Text_new();

        Text sut = Text_overwriteWithFormat(&tmp, "%02d lorem %s%c", 5, "ipsum", '\n');
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), strlen("05 lorem ipsum\n"));
        assert_string_equal(sut, "05 lorem ipsum\n");

        Text_delete(sut);
    }

    {
        Text tmp = Text_fromLiteral("lorem ipsum");

        Text sut = Text_overwriteWithFormat(&tmp, "lorem %02d %s%c", 5, "ipsum", '\n');
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), strlen("lorem 05 ipsum\n"));
        assert_string_equal(sut, "lorem 05 ipsum\n");

        Text_delete(sut);
    }

    // TODO test expansion
}

Feature(overwriteWithFormat_checkRuntimeErrors) {
    Text sut = NULL;
    const char *format = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_overwriteWithFormat(&sut, "%s", "lorem");
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_new();
    traits_unit_wraps(SIGABRT) {
        sut = Text_overwriteWithFormat(&sut, format, NULL);
        (void) sut;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    Text_delete(sut);
}

Feature(vOverwriteWithFormat) {

}

Feature(vOverwriteWithFormat_checkRuntimeErrors) {

}

Feature(overwriteWithBytes) {
    Text sut = NULL, tmp = Text_withCapacity(0);

    const struct ByteArray bytesArray[] = {
            {.bytes="", .size=0},
            {.bytes="Hello world!", .size=12},
            {.bytes="sit,\0amet,\0consectetur,\0adipisci", .size=32}
    };
    const size_t bytesArraySize = sizeof(bytesArray) / sizeof(bytesArray[0]);

    for (size_t i = 0; i < bytesArraySize; i++) {
        const void *bytes = bytesArray[i].bytes;
        const size_t size = bytesArray[i].size;

        sut = Text_overwriteWithBytes(&tmp, bytes, size);
        assert_null(tmp);
        tmp = sut;

        assert_greater_equal(Text_capacity(sut), Text_length(sut));
        assert_equal(size, Text_length(sut));
        assert_memory_equal(size, bytes, sut);
    }

    Text_delete(sut);
}

Feature(overwriteWithBytes_checkRuntimeErrors) {
    Text sut;
    const void *bytesArray;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {   // sut is NULL
        sut = NULL;
        bytesArray = "lorem ipsum";
        traits_unit_wraps(SIGABRT) {
            sut = Text_overwriteWithBytes(&sut, bytesArray, 8);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    }

    {   // bytesArray is NULL
        sut = Text_withCapacity(0);
        bytesArray = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_overwriteWithBytes(&sut, bytesArray, 8);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    {   // size is SIZE_MAX
        sut = Text_withCapacity(0);
        bytesArray = "lorem ipsum";
        traits_unit_wraps(SIGABRT) {
            sut = Text_overwriteWithBytes(&sut, bytesArray, SIZE_MAX);
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(overwriteWithLiteral) {
    Text sut = NULL, tmp = Text_withCapacity(0);
    const char *literals[] = {"", "Hello world!", "lorem ipsum dolor", "sit,\0amet,\0consectetur,\0adipisci"};
    const size_t literalsSize = sizeof(literals) / sizeof(literals[0]);

    for (size_t i = 0; i < literalsSize; i++) {
        const char *literal = literals[i];
        const size_t length = strlen(literal);

        sut = Text_overwriteWithLiteral(&tmp, literal);
        assert_null(tmp);
        tmp = sut;

        assert_greater_equal(Text_capacity(sut), Text_length(sut));
        assert_equal(length, Text_length(sut));
        assert_string_equal(literal, sut);
    }

    Text_delete(sut);
}

Feature(overwriteWithLiteral_checkRuntimeErrors) {
    Text sut = NULL;
    const char *literal = "lorem ipsum";
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_overwriteWithLiteral(&sut, literal);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_withCapacity(0);
    literal = NULL;
    traits_unit_wraps(SIGABRT) {
        sut = Text_overwriteWithLiteral(&sut, literal);
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    Text_delete(sut);
}

Feature(append) {
    {
        Text seed = Text_new(), tmp = Text_new();

        Text sut = Text_append(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), 0);
        assert_string_equal(sut, "");

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromLiteral(" ipsum"), tmp = Text_new();

        Text sut = Text_append(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), strlen(" ipsum"));
        assert_string_equal(sut, " ipsum");

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_fromLiteral(" ipsum"), tmp = Text_new();

        Text sut = Text_append(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), strlen(" ipsum"));
        assert_string_equal(sut, " ipsum");

        Text_delete(sut);
        Text_delete(seed);
    }

    {
        Text seed = Text_new(), tmp = Text_fromLiteral("lorem");

        Text sut = Text_append(&tmp, seed);
        assert_null(tmp);
        assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        assert_equal(Text_length(sut), strlen("lorem"));
        assert_string_equal(sut, "lorem");

        Text_delete(sut);
        Text_delete(seed);
    }

    // TODO test expansion
}

Feature(append_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {
        Text sut = NULL, other = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_append(&sut, other);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
        Text_delete(other);
    }

    {
        Text sut = Text_new(), other = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_append(&sut, other);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(appendFormat) {
#define FORMAT  "%02d %s %c !", 5, "lorem", 'a'

    size_t n;
    Text sut = Text_new(), tmp = NULL;
    const char seed[] = "05 lorem a !";
    const size_t startCapacity = Text_capacity(sut), seedSize = (sizeof(seed) / sizeof(seed[0])) - 1;

    for (n = 0; Text_length(sut) <= startCapacity; n++) {
        assert_equal(Text_length(sut), n * seedSize);
        assert_equal(Text_capacity(sut), startCapacity);
        if (n > 0) {
            for (size_t i = 0; i < n; i++) {
                assert_memory_equal(seedSize, sut + (i * seedSize), seed);
            }
        } else {
            assert_string_equal(sut, "");
        }
        tmp = Text_appendFormat(&sut, FORMAT);
        assert_null(sut);
        sut = tmp;
    }

    for (const size_t k = n + 5; n < k; n++) {
        assert_equal(Text_length(sut), n * seedSize);
        assert_greater(Text_capacity(sut), startCapacity);
        for (size_t i = 0; i < n; i++) {
            assert_memory_equal(seedSize, sut + (i * seedSize), seed);
        }
        tmp = Text_appendFormat(&sut, FORMAT);
        assert_null(sut);
        sut = tmp;
    }

    Text_delete(sut);

#undef FORMAT
}

Feature(appendFormat_checkRuntimeErrors) {
    Text sut = NULL;
    const char *format = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_appendFormat(&sut, "%s", "lorem");
        (void) sut;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_new();
    traits_unit_wraps(SIGABRT) {
        sut = Text_appendFormat(&sut, format, NULL);
        (void) sut;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    Text_delete(sut);
}

Feature(vAppendFormat) {

}

Feature(vAppendFormat_checkRuntimeErrors) {

}

Feature(appendBytes) {
    Text sut = Text_withCapacity(0), tmp = NULL;

    {
        const char bytes[] = "lo\0rem";
        const size_t size = sizeof(bytes) - 1;

        tmp = Text_appendBytes(&sut, "lo\0rem", sizeof("lo\0rem") - 1);
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_memory_equal(size, bytes, sut);
    }

    {
        const char bytes[] = "lo\0remip\0sum";
        const size_t size = sizeof(bytes) - 1;

        tmp = Text_appendBytes(&sut, "ip\0sum", sizeof("ip\0sum") - 1);
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_memory_equal(size, bytes, sut);
    }

    {
        const char bytes[] = "lo\0remip\0sumdo\0lor";
        const size_t size = sizeof(bytes) - 1;

        tmp = Text_appendBytes(&sut, "do\0lor", sizeof("do\0lor") - 1);
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_memory_equal(size, bytes, sut);
    }

    Text_delete(sut);

    // TODO test expansion
}

Feature(appendBytes_checkRuntimeErrors) {
    Text sut;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {
        const char bytes[] = "lorem\0ipsum";
        sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_appendBytes(&sut, bytes, sizeof(bytes) - 1);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    }

    {
        const char bytes[] = "lorem\0ipsum";
        sut = Text_withCapacity(0);
        traits_unit_wraps(SIGABRT) {
            sut = Text_appendBytes(&sut, bytes, SIZE_MAX);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    {
        const char *bytes = NULL;
        sut = Text_withCapacity(0);
        traits_unit_wraps(SIGABRT) {
            sut = Text_appendBytes(&sut, bytes, 8);
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(appendLiteral) {
    size_t size;
    const char *literal;
    Text sut = Text_withCapacity(0), tmp = NULL;

    {
        literal = "lorem";
        size = strlen(literal);

        tmp = Text_appendLiteral(&sut, "lorem");
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_string_equal(literal, sut);
    }

    {
        literal = "loremipsum";
        size = strlen(literal);

        tmp = Text_appendLiteral(&sut, "ipsum");
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_string_equal(literal, sut);
    }

    {
        literal = "loremipsumdolor";
        size = strlen(literal);

        tmp = Text_appendLiteral(&sut, "dolor");
        assert_null(sut);
        sut = tmp;

        assert_equal(size > TEXT_DEFAULT_CAPACITY ? size : TEXT_DEFAULT_CAPACITY, Text_capacity(sut));
        assert_equal(size, Text_length(sut));
        assert_string_equal(literal, sut);
    }

    Text_delete(sut);
}

Feature(appendLiteral_checkRuntimeErrors) {
    Text sut;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {
        sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_appendLiteral(&sut, "lorem ipsum");
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    }

    {
        const char *literal = NULL;
        sut = Text_withCapacity(0);
        traits_unit_wraps(SIGABRT) {
            sut = Text_appendLiteral(&sut, literal);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(insert) {
    const void *expected = "Hello world!";
    const size_t expectedSize = strlen(expected);

    {
        Text sut = Text_new(), text = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insert(&sut, 0, text);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), 0);
        assert_equal(Text_capacity(sut), capacity);
        assert_string_equal(sut, text);

        Text_delete(text);
        Text_delete(sut);
    }

    {   // prepend
        Text sut = Text_new(), text = Text_fromLiteral(expected), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insert(&sut, 0, text);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(text);
        Text_delete(sut);
    }

    {   // insert middle
        Text sut = Text_fromLiteral("Helld!"), text = Text_fromLiteral("lo wor"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insert(&sut, 3, text);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(text);
        Text_delete(sut);
    }

    {   // append
        Text sut = Text_fromLiteral("Hello"), text = Text_fromLiteral(" world!"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insert(&sut, Text_length(sut), text);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(text);
        Text_delete(sut);
    }
}

Feature(insert_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    { // ref and *ref must not be NULL.
        Text sut = NULL, text = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insert(&sut, 0, text);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

        Text *ref = &sut;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insert(ref, 0, text);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(text);
    }

    { // index must be less or equal than the length of the text.
        Text sut = Text_new(), text = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insert(&sut, 1, text);
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(text);
        Text_delete(sut);
    }

    { // text must not be NULL.
        Text sut = Text_new(), text = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insert(&sut, 0, text);
        }
        assert_equal(counter + 4, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(insertFormat) {
    const char expected[] = "Hello world!";
    const size_t expectedSize = (sizeof(expected) / sizeof(expected[0])) - 1;

    {
        Text sut = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertFormat(&sut, 0, "%s", "");
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), 0);
        assert_equal(Text_capacity(sut), capacity);
        assert_string_equal(sut, "");

        Text_delete(sut);
    }

    {   // prepend
        Text sut = Text_fromLiteral("world!"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertFormat(&sut, 0, "%s%c", "Hello", ' ');
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // insert middle
        Text sut = Text_fromLiteral("Helld!"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertFormat(&sut, 3, "%s%c%s", "lo", ' ', "wor");
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // append
        Text sut = Text_fromLiteral("Hello"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertFormat(&sut, Text_length(sut), " %s%c", "world", '!');
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }
}

Feature(insertFormat_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    { // ref and *ref must not be NULL.
        Text sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertFormat(&sut, 0, "%s", "");
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

        Text *ref = &sut;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertFormat(ref, 0, "%s", "");
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    }

    { // index must be less or equal than the length of the text.
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertFormat(&sut, 1, "%s", "");
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    { // format must not be NULL.
        const char *format = NULL;
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertFormat(&sut, 0, format, "");
        }
        assert_equal(counter + 4, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(vInsertFormat) {

}

Feature(vInsertFormat_checkRuntimeErrors) {

}

Feature(insertBytes) {
    const char expected[] = "Hello\0world!";
    const size_t expectedSize = (sizeof(expected) / sizeof(expected[0])) - 1;

    {
        Text sut = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertBytes(&sut, 0, "", 0);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), 0);
        assert_equal(Text_capacity(sut), capacity);
        assert_string_equal(sut, "");

        Text_delete(sut);
    }

    {   // prepend
        Text sut = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertBytes(&sut, 0, expected, expectedSize);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // insert middle
        Text sut = Text_fromLiteral("Helld!"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertBytes(&sut, 3, "lo\0wor", 6);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // append
        Text sut = Text_fromLiteral("Hello"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertBytes(&sut, Text_length(sut), "\0world!", 7);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }
}

Feature(insertBytes_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    { // ref and *ref must not be NULL.
        Text sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertBytes(&sut, 0, "", 0);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

        Text *ref = &sut;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertBytes(ref, 0, "", 0);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    }

    { // index must be less or equal than the length of the text.
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertBytes(&sut, 1, "", 0);
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    { // bytes must not be NULL.
        const void *bytes = NULL;
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertBytes(&sut, 0, bytes, 0);
        }
        assert_equal(counter + 4, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    { // size must be less than SIZE_MAX.
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertBytes(&sut, 0, "", SIZE_MAX);
        }
        assert_equal(counter + 5, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(insertLiteral) {
    const void *expected = "Hello world!";
    const size_t expectedSize = strlen(expected);

    {
        Text sut = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertLiteral(&sut, 0, "");
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), 0);
        assert_equal(Text_capacity(sut), capacity);
        assert_string_equal(sut, "");

        Text_delete(sut);
    }

    {   // prepend
        Text sut = Text_new(), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertLiteral(&sut, 0, expected);
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // insert middle
        Text sut = Text_fromLiteral("Helld!"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertLiteral(&sut, strlen("Hel"), "lo wor");
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }

    {   // append
        Text sut = Text_fromLiteral("Hello"), tmp;
        const size_t capacity = Text_capacity(sut);

        tmp = Text_insertLiteral(&sut, Text_length(sut), " world!");
        assert_null(sut);
        sut = tmp;

        assert_equal(Text_length(sut), expectedSize);
        assert_equal(Text_capacity(sut), capacity);
        assert_memory_equal(expectedSize, sut, expected);

        Text_delete(sut);
    }
}

Feature(insertLiteral_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    { // ref and *ref must not be NULL.
        Text sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertLiteral(&sut, 0, "");
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

        Text *ref = &sut;
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertLiteral(ref, 0, "");
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
    }

    { // index must be less or equal than the length of the text.
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertLiteral(&sut, 1, "");
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    { // literal must not be NULL.
        const void *literal = NULL;
        Text sut = Text_new();
        traits_unit_wraps(SIGABRT) {
            sut = Text_insertLiteral(&sut, 0, literal);
        }
        assert_equal(counter + 4, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(eraseRange) {
    {   // no-op
        const char *content = "lorem";
        Text sut = Text_fromLiteral(content);
        const size_t length = Text_length(sut), capacity = Text_capacity(sut);

        Text_eraseRange(sut, 0, 0);
        assert_equal(length, Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal(content, sut);

        Text_eraseRange(sut, length, length);
        assert_equal(length, Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal(content, sut);

        Text_delete(sut);
    }

    {   // erase from begin
        Text sut = Text_fromLiteral("Hello world!");
        const size_t capacity = Text_capacity(sut);

        Text_eraseRange(sut, 0, strlen("Hello "));
        assert_equal(strlen("world!"), Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal("world!", sut);

        Text_delete(sut);
    }

    {   // erase middle
        Text sut = Text_fromLiteral("Hello world!");
        const size_t capacity = Text_capacity(sut);

        Text_eraseRange(sut, strlen("Hello"), Text_length(sut) - 1);
        assert_equal(strlen("Hello!"), Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal("Hello!", sut);

        Text_delete(sut);
    }

    {   // erase till end
        Text sut = Text_fromLiteral("Hello world!");
        const size_t capacity = Text_capacity(sut);

        Text_eraseRange(sut, strlen("Hello"), Text_length(sut));
        assert_equal(strlen("Hello"), Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal("Hello", sut);

        Text_delete(sut);
    }

    {   // erase all
        Text sut = Text_fromLiteral("Hello world!");
        const size_t capacity = Text_capacity(sut);

        Text_eraseRange(sut, 0, Text_length(sut));
        assert_equal(0, Text_length(sut));
        assert_equal(capacity, Text_capacity(sut));
        assert_string_equal("", sut);

        Text_delete(sut);
    }
}

Feature(eraseRange_checkRuntimeErrors) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    { // self must not be NULL.
        Text sut = NULL;
        traits_unit_wraps(SIGABRT) {
            Text_eraseRange(sut, 0, 0);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    }

    { // start must be less or equal than end.
        Text sut = Text_fromLiteral("lorem");
        traits_unit_wraps(SIGABRT) {
            Text_eraseRange(sut, 1, 0);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }

    { // end must be less or equal than the length of the text.
        Text sut = Text_fromLiteral("lorem");
        traits_unit_wraps(SIGABRT) {
            Text_eraseRange(sut, 0, sizeof("lorem") + 1);
        }
        assert_equal(counter + 3, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(quote) {
    Text tmp = NULL;

    {
        const char BYTES[] = "";
        const char EXPECTED[] = "\"\"";
        const size_t BYTES_SIZE = 0;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        Text sut = Text_fromBytes(BYTES, BYTES_SIZE);
        tmp = Text_quote(&sut);
        assert_null(sut);
        sut = tmp;

        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

        Text_delete(sut);
    }

    {
        const char BYTES[] = " \" \\ / \b \f \n \r \t \0 ";
        const char EXPECTED[] = "\" \\\" \\\\ \\/ \\b \\f \\n \\r \\t \\u0000 \"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        Text sut = Text_fromBytes(BYTES, BYTES_SIZE);
        tmp = Text_quote(&sut);
        assert_null(sut);
        sut = tmp;

        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

        Text_delete(sut);
    }

    {
        const char BYTES[] = "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o";
        const char EXPECTED[] = "\"a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        Text sut = Text_fromBytes(BYTES, BYTES_SIZE);
        tmp = Text_quote(&sut);
        assert_null(sut);
        sut = tmp;

        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

        Text_delete(sut);
    }

    {
        const char BYTES[] = "1a\"2b\\3c/4d\b5e\f6f\n7g\r8h\t9i\0" "10l\1" "11m\2" "12n\3" "13o";
        const char EXPECTED[] = "\"1a\\\"2b\\\\3c\\/4d\\b5e\\f6f\\n7g\\r8h\\t9i\\u000010l\\u000111m\\u000212n\\u000313o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_less(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = TEXT_DEFAULT_CAPACITY;

        Text sut = Text_fromBytes(BYTES, BYTES_SIZE);
        tmp = Text_quote(&sut);
        assert_null(sut);
        sut = tmp;

        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

        Text_delete(sut);
    }

    {
        const char BYTES[] = "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o" "a\"b\\c/d\be\ff\ng\rh\ti\0l\1m\2n\3o";
        const char EXPECTED[] = "\"a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o" "a\\\"b\\\\c\\/d\\be\\ff\\ng\\rh\\ti\\u0000l\\u0001m\\u0002n\\u0003o\"";
        const size_t BYTES_SIZE = sizeof(BYTES) - 1;
        const size_t EXPECTED_SIZE = sizeof(EXPECTED) - 1;
        assert_greater(EXPECTED_SIZE, TEXT_DEFAULT_CAPACITY);
        const size_t EXPECTED_CAPACITY = EXPECTED_SIZE;

        Text sut = Text_fromBytes(BYTES, BYTES_SIZE);
        tmp = Text_quote(&sut);
        assert_null(sut);
        sut = tmp;

        assert_not_null(sut);
        assert_string_equal(sut, EXPECTED);
        assert_equal(Text_length(sut), EXPECTED_SIZE);
        assert_greater_equal(Text_capacity(sut), EXPECTED_CAPACITY);

        Text_delete(sut);
    }
}

Feature(quote_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_quote(&sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(lower) {
    Text sut = Text_fromBytes(
            "0123456789\0abcdefghijklmnopqrstuvwxyz\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \0\t\n\r",
            102
    );

    const size_t length = Text_length(sut), capacity = Text_capacity(sut);
    Text_lower(sut);
    assert_equal(capacity, Text_capacity(sut));
    assert_equal(length, Text_length(sut));
    assert_memory_equal(
            102,
            "0123456789\0abcdefghijklmnopqrstuvwxyz\0abcdefghijklmnopqrstuvwxyz\0!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \0\t\n\r",
            sut
    );

    Text_delete(sut);
}

Feature(lower_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text_lower(sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(upper) {
    Text sut = Text_fromBytes(
            "0123456789\0abcdefghijklmnopqrstuvwxyz\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \0\t\n\r",
            102
    );

    const size_t length = Text_length(sut), capacity = Text_capacity(sut);
    Text_upper(sut);
    assert_equal(capacity, Text_capacity(sut));
    assert_equal(length, Text_length(sut));
    assert_memory_equal(
            102,
            "0123456789\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \0\t\n\r",
            sut
    );

    Text_delete(sut);
}

Feature(upper_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text_upper(sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(clear) {
    Text sut = Text_withCapacity(0);

    sut = Text_overwriteWithLiteral(&sut, "Hello world!");
    const size_t capacity = Text_capacity(sut);

    Text_clear(sut);
    assert_equal(capacity, Text_capacity(sut));
    assert_equal(0, Text_length(sut));
    assert_string_equal("", sut);

    Text_delete(sut);
}

Feature(clear_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text_clear(sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(setLength) {
    Text sut = Text_new();
    const size_t capacity = Text_capacity(sut);

    Text_setLength(sut, capacity);
    assert_equal(capacity, Text_length(sut));
    assert_equal(capacity, Text_capacity(sut));
    assert_equal('\0', sut[capacity]);

    sut = Text_overwriteWithLiteral(&sut, "Hello World!");
    Text_setLength(sut, strlen("Hello"));
    assert_string_equal("Hello", sut);
    assert_equal(strlen("Hello"), Text_length(sut));
    assert_equal(capacity, Text_capacity(sut));

    Text_delete(sut);
}

Feature(setLength_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text_setLength(sut, 0);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_new();

    traits_unit_wraps(SIGABRT) {
        Text_setLength(sut, Text_capacity(sut) + 1);
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    Text_delete(sut);
}

Feature(expandToFit) {
    const char *content = "lorem ipsum";
    const size_t size = strlen(content);
    Text sut = Text_fromLiteral(content), tmp = NULL;

    for (size_t i = 1; i <= 6; ++i) {
        {
            const size_t capacity = Text_capacity(sut);
            tmp = Text_expandToFit(&sut, capacity - 1);
            assert_null(sut);
            sut = tmp;

            assert_equal(capacity, Text_capacity(sut));
            assert_equal(size, Text_length(sut));
            assert_string_equal(content, sut);
        }

        {
            const size_t capacity = Text_capacity(sut);
            tmp = Text_expandToFit(&sut, capacity);
            assert_null(sut);
            sut = tmp;

            assert_equal(capacity, Text_capacity(sut));
            assert_equal(size, Text_length(sut));
            assert_string_equal(content, sut);
        }

        {
            const size_t capacity = Text_capacity(sut);
            tmp = Text_expandToFit(&sut, capacity + i);
            assert_null(sut);
            sut = tmp;

            assert_greater(Text_capacity(sut), capacity);
            assert_equal(size, Text_length(sut));
            assert_string_equal(content, sut);
        }

        {
            const size_t capacity = Text_capacity(sut);
            tmp = Text_expandToFit(&sut, capacity / 4);
            assert_null(sut);
            sut = tmp;

            assert_equal(capacity, Text_capacity(sut));
            assert_equal(size, Text_length(sut));
            assert_string_equal(content, sut);
        }
    }

    Text_delete(sut);
}

Feature(expandToFit_checkRuntimeErrors) {
    Text sut;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    {
        sut = NULL;
        traits_unit_wraps(SIGABRT) {
            sut = Text_expandToFit(&sut, 8);
        }
        assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    }

    {
        sut = Text_withCapacity(0);
        traits_unit_wraps(SIGABRT) {
            sut = Text_expandToFit(&sut, SIZE_MAX);
        }
        assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());
        Text_delete(sut);
    }
}

Feature(shrinkToFit) {
    Text sut = Text_withCapacity(0), tmp = NULL;
    const char *literal = "lorem";
    const size_t length = strlen(literal);

    sut = Text_overwriteWithLiteral(&sut, "lorem ipsum dolor");
    sut = Text_overwriteWithLiteral(&sut, literal);

    tmp = Text_shrinkToFit(&sut);
    assert_null(sut);
    sut = tmp;

    assert_equal(length, Text_capacity(sut));
    assert_equal(length, Text_length(sut));
    assert_string_equal(literal, sut);

    Text_delete(sut);
}

Feature(shrinkToFit_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_shrinkToFit(&sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(push) {
    char s[TEXT_DEFAULT_CAPACITY + 13];
    const size_t size = (sizeof(s) / sizeof(s[0])) - 1;
    Text sut = Text_new();

    memset(s, 0, size + 1);
    for (size_t i = 0; i < size; i++) {
        s[i] = 'a';
        sut = Text_push(&sut, 'a');
        assert_equal(strlen(s), Text_length(sut));
        assert_string_equal(s, sut);
        if (Text_length(sut) <= TEXT_DEFAULT_CAPACITY) {
            assert_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        } else {
            assert_greater_equal(Text_capacity(sut), TEXT_DEFAULT_CAPACITY);
        }
    }

    Text_delete(sut);
}

Feature(push_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        sut = Text_push(&sut, 'a');
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}


Feature(pop) {
    const char s[] = "Hello World!";
    const size_t size = sizeof(s) / sizeof(s[0]) - 1;
    Text sut = Text_fromLiteral(s);

    for (size_t i = 1; i <= size; i++) {
        const char c = Text_pop(sut);
        assert_equal(c, s[size - i]);
        assert_equal(size - i, Text_length(sut));
    }

    Text_delete(sut);
}

Feature(pop_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Text_pop(sut);
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_new();

    traits_unit_wraps(SIGABRT) {
        Text_pop(sut);
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    Text_delete(sut);
}

Feature(put) {
    const char literal[] = "lorem ipsum";
    const size_t size = sizeof(literal) - 1;
    Text sut = Text_fromLiteral(literal);

    for (size_t i = 0; i < size; i++) {
        assert_equal(literal[i], Text_put(sut, i, 'x'));
        assert_equal('x', sut[i]);
    }

    Text_delete(sut);
}

Feature(put_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const char _ = Text_put(sut, 8, 'x');
        (void) _;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_fromLiteral("lorem");

    traits_unit_wraps(SIGABRT) {
        const char _ = Text_put(sut, SIZE_MAX, 'x');
        (void) _;
    }
    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    Text_delete(sut);
}

Feature(get) {
    const char literal[] = "lorem ipsum";
    const size_t size = sizeof(literal) - 1;
    Text sut = Text_fromLiteral(literal);

    for (size_t i = 0; i < size; i++) {
        assert_equal(literal[i], Text_get(sut, i));
    }

    Text_delete(sut);
}

Feature(get_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const char _ = Text_get(sut, 8);
        (void) _;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    sut = Text_fromLiteral("lorem");

    traits_unit_wraps(SIGABRT) {
        const char _ = Text_get(sut, SIZE_MAX);
        (void) _;
    }
    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    Text_delete(sut);
}

Feature(length_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const size_t length = Text_length(sut);
        (void) length;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(capacity_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const size_t capacity = Text_capacity(sut);
        (void) capacity;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
}

Feature(equals) {
    Text sut, other;

    sut = Text_fromLiteral("lorem ipsum");
    assert_true(Text_equals(sut, sut));

    other = Text_fromBytes("lorem\0ipsum", sizeof("lorem\0ipsum") - 1);
    assert_equal(Text_length(sut), Text_length(other));
    assert_false(Text_equals(sut, other));
    assert_equal('\0', Text_put(other, 5, ' '));
    assert_true(Text_equals(sut, sut));

    Text_delete(sut);
    Text_delete(other);
}

Feature(equals_checkRuntimeErrors) {
    Text sut = Text_fromLiteral("lorem"), other = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const bool r = Text_equals(sut, other);
        (void) r;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());

    traits_unit_wraps(SIGABRT) {
        const bool r = Text_equals(other, sut);
        (void) r;
    }

    assert_equal(counter + 2, traits_unit_get_wrapped_signals_counter());

    Text_delete(sut);
}

Feature(isEmpty) {
    Text sut = Text_new();
    assert_true(Text_isEmpty(sut));

    sut = Text_appendLiteral(&sut, "lorem ipsum");
    assert_false(Text_isEmpty(sut));

    Text_delete(sut);
}

Feature(isEmpty_checkRuntimeErrors) {
    Text sut = NULL;
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        const bool r = Text_isEmpty(sut);
        (void) r;
    }

    assert_equal(counter + 1, traits_unit_get_wrapped_signals_counter());
    Text_delete(sut);
}
