# Операции `as_signed()`, `as_unsigned()`

Операции `as_signed()`, `as_unsigned()` предназначены для явной смены интерпретации значения целочисленного выражения на знаковое или беззнаковое. Разрядность выражения остаётся неизменной.

Проверка значения на соответствие границам области значений целевого типа гарантированно не производится, даже в `checked` контексте.

Если дано некоторое `x`, то строка `auto y = as_signed(x)` эквивалентна следующему:

```
static_assert(is_signed(x) | is_unsigned(x));
auto y = reinterpret_cast(x, signed(bitsizeof(x)));
static_assert(is_signed(y));
static_assert(bitsizeof(y) == bitsizeof(x));
```

Аналогично для `auto y = as_unsigned(x)`:

```
static_assert(is_signed(x) | is_unsigned(x));
auto y = reinterpret_cast(x, unsigned(bitsizeof(x)));
static_assert(is_unsigned(y));
static_assert(bitsizeof(y) == bitsizeof(x));
```

