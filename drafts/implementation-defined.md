# Поведение, определяемое реализацией

## Пространство имён `qod.optional`

Пространство имён `qod.optional` содержит имена, описывающие характеристики реализации согласно данной спецификации.

## Пространство имён `qod.impl`

Содержимое пространства имён `qod.impl` определяется реализацией.

## Пространство имён `qod.env`

Пространство имён `qod.env` предназначено для передачи во время компиляции дополнительной информации в компилируемую программу способом, который определяется реализаций.

Реализация должна специфицировать способ передачи дополнительной информации, если реализация поддерживает такую возможность.

Также реализация должна специфицировать, какие имена (и их предназначение) в `qod.env` она определяет имплицитно, если таковые имеются.

## Доступость hosted окружения

Булева константа `qod.optional.hosted` содержит `true`, если при компиляции доступно hosted окружение, в ином случае - `false`.

## Булевы типы данных

Размер следующего типа данных определяется реализацией: `bool`.

Константа `qod.optional.boolean_max_size` содержит максимальный размер в байтах для конструктора типа данных `boolean(n)`, поддерживаемый реализацией.

## Целочисленные типы данных

Размер следующих типов данных определяется реализацией:

```
int
long
intsz
intptr
uint
ulong
uintsz
uintptr
word
```

Константа `qod.optional.int_max_size` содержит максимальный размер в байтах для конструкторов типов данных `signed(n)` и `unsigned(n)`, поддерживаемый реализацией.

Данная константа может содержать размер типа, для которого реализация в принципе способна сгенерировать корректный программный код, но это не даёт гарантий, что такой программный код может быть исполнен на целевой машине (в связи с ограничениями на объем физической памяти, ограничением ресурсов со стороны операционной системы и т.п.).

При сборке во freestanding окружении реализация может формировать обращения к внешним именам для выполнения операций с данными типами. В таком случае перечень и назначение имён должны быть определены реализацией.

## Целочисленные типы данных с NaN

Булева константа `qod.optional.int_with_nan` содержит `true`, если реализация поддерживает целочисленные типы данных с NaN и операции над ними, в ином случае - `false`.

Если `qod.optional.int_with_nan == false`, реализация не обязана определять имена, описанные далее в подразделе, а также операции над соответствующими типами данных.

Если реализация поддерживает целочисленные типы данных с NaN, следующие имена должны быть определены:

Конструкторы типов `signed_with_nan(n)` и `unsigned_with_nan(n)`.

Указанные псевдонимы типов:

```
type wn_int8    = signed_with_nan(8);
type wn_int16   = signed_with_nan(16);
type wn_int32   = signed_with_nan(32); /* только если qod.optional.int_with_nan_max_size >= 32 */
type wn_int64   = signed_with_nan(64); /* только если qod.optional.int_with_nan_max_size >= 64 */
type wn_int128  = signed_with_nan(128); /* только если qod.optional.int_with_nan_max_size >= 128 */

type wn_uint8   = unsigned_with_nan(8);
type wn_uint16  = unsigned_with_nan(16);
type wn_uint32  = unsigned_with_nan(32); /* только если qod.optional.int_with_nan_max_size >= 32 */
type wn_uint64  = unsigned_with_nan(64); /* только если qod.optional.int_with_nan_max_size >= 64 */
type wn_uint128 = unsigned_with_nan(128); /* только если qod.optional.int_with_nan_max_size >= 128 */
```

Константа `qod.optional.int_with_nan_max_size` содержит максимальный размер в байтах для типов данных `signed_with_nan(n)` и `unsigned_with_nan(n)`, поддерживаемый реализацией.

Если реализация поддерживает целочисленные типы данных с NaN, при сборке во freestanding окружении реализация может формировать обращения к внешним именам для выполнения операций с данными типами. В таком случае перечень и назначение имён должны быть определены реализацией.

## Типы данных с плавающей точкой и операции над ними

Булева константа `qod.optional.float` содержит `true`, если реализация поддерживает типы с плавающей точкой и операции над ними, в ином случае - `false`.

Если `qod.optional.float == false`, реализация не обязана определять следующие типы данных, а также любые операции над ними:

```
float
double
long_double
```

Если реализация поддерживает типы с плавающей точкой, при сборке во freestanding окружении реализация может формировать обращения к внешним именам для выполнения операций с данными типами. В таком случае перечень и назначение имён должны быть определены реализацией.
