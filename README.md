# Современные технологии программирования
## [Проект «Full-text search»](https://github.com/kiryu-dev/full-text-search)
Ваша задача — в течение семестра реализовать систему полнотекстового поиска.  
В качестве источника документов будем использовать базу книг: books.csv.  
Проект должен состоять из следующих компонентов:  
1. Ядро. Библиотека или несколько библиотек на языке С++, в которых реализована индексация, поиск и ранжирование документов.
2. Консольные утилиты для использования библиотеки (С++).
3. Обертка на языке С для кроссязыкового использования библиотеки.
4. Клиент для поиска, написанный на высокоуровневом языке программирования на ваш выбор: Python, Go, PHP, Java, … При выборе найдите инструменты для вызова функций, написанных на C. Ключевое слово для поиска: FFI.
5. Вся функциональность должна быть покрыта unit-тестами.

При разработке проекта:
1. Код пишется на C++17.
2. Используется система сборки CMake.
3. Исходники хранятся в git-репозитории. Процесс ведется с соблюдением [A simple git branching model](https://gist.github.com/jbenet/ee6c9ac48068889b0912).
4. Код автоматически форматируется с помощью `clang-format`.
5. При сборке код проходит проверки `clang-tidy`.

Демо:
1. Выдача перестраивается при уточнении запроса.
2. Поиск способен обрабатывать запросы с опечатками.

## 1. Создание проекта CMake
### Цель
Создать проект, включающий следующие компоненты:
1. Библиотеку (статическую или динамическую).
2. Unit-тесты.
3. Консольное приложение.
4. Сторонние библиотеки.
5. Конфиг clang-tidy.
6. Конфиг clang-format.

Для сборки проекта используется CMake. В процессе сборки должен выполняться статический анализ кода.  
Целевые версии ПО в рамках курса:
1. gcc >= 9.3.0
2. llvm >= 12.0.1 (clang, clang-tidy, clang-format)
3. cmake >= 3.21

### Материалы
1. [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
2. [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
3. [Clang-Tidy](https://releases.llvm.org/12.0.0/tools/clang/tools/extra/docs/clang-tidy/)

### Структура проекта
Для C++ нет единого общепринятого способа структурировать проекты. Существует несколько рекомендаций.
1. [Canonical Project Structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html)
2. [The Pitchfork Layout (PFL)](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs) — предлагает различные варианты расположения заголовочных файлов и тестов:
    1. Separate header placement
    2. Merged header placement
    3. Separate test placement
    4. Merged test placement
    
### Clang Tidy
Clang Tidy — статический анализатор кода. Его использование поможет избежать ряда ошибок в коде, а в некоторых случаях компенсирует незнание некоторых особенностей языка.

На данный момент реализовано 300+ различных проверок. Диагностики объединены в группы: специфичные для конкретных проектов (`abseil-*`, `llvm-*` и др.), кодстайлов (`google-*`, `hicpp-*`) и прочие (`readability-*`, `bugprone-*`).

По-хорошему нужно сгенерировать когфиг (`clang-tidy --dump-config`), затем вручную вписать все нужные для вашего проекта опции. Тогда вы получите конфиг, устойчивый к обновлению clang-tidy (при переходе на новую версию не появится новых предупреждений, пока вы не добавите соответствующие диагностики в конфиг). Однако такой процесс слишком трудоемок, поэтому в рамках курса предлагаем другой подход.
1. Сгенерируйте конфиг со всеми включенными проверками:
```
clang-tidy --checks='*' --dump-config > .clang-tidy
```
2. По мере разработки проекта отключайте ненужные проверки, если они срабатывают.

Понятно, что каждое диагностическое сообщение нужно вдумчиво анализировать, и в случае наличия реальной проблемы в коде исправлять ее, а не глушить предупреждение.

Примеры некоторых диагностик, которые в рамках курса можно отключить сразу:
1. `fuchsia-*` — проверки, специфичные для [кодстайла проекта fuchsia](https://fuchsia.dev/fuchsia-src/development/languages/c-cpp/cpp-style).
2. `llvmlibc-*`
3. llvm-header-guard — в нашем курсе мы используем `#pragma once`
4. modernize-use-nodiscard
5. modernize-use-trailing-return-type — такой стиль мы будем использовать только по необходимости или для заметного сокращения кода.
6. cppcoreguidelines-owning-memory — проверка требует использования библиотеки [GSL](https://github.com/microsoft/GSL)
7. cppcoreguidelines-pro-bounds-pointer-arithmetic
8. cppcoreguidelines-avoid-magic-numbers — проверка слишком шумит на тестах. В коде руководствуйтесь здравым смыслом.

Таким образом, начало конфига будет выглядеть так:
```
---
Checks: >
  clang-diagnostic-*,
  clang-analyzer-*,
  *,
  -fuchsia-*,
  -llvmlibc-*,
  -llvm-header-guard,
  -modernize-use-nodiscard,
  -modernize-use-trailing-return-type,
  -cppcoreguidelines-owning-memory,
  -google-runtime-references,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  -cppcoreguidelines-avoid-magic-number,
  -readability-magic-numbers
```

### Использование сторонних библиотек
В рамках курса мы будем использовать один из двух способов подключения сторонних библиотек. С точки зрения структуры проекта они выглядят почти одинаково. Сторонние библиотеки располагаются в каталоге `<project>/external`:
```
<project>
|
`-- external
    |-- CMakeLists.txt
    |-- cxxopts
    |-- fmtlib
    `-- googletest
```
Файл `external/CMakeLists.txt` содержит только подключение дочерних каталогов.

### FetchContent
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) — cmake-модуль для скачивания внешних проектов и автоматического добавления их как подпроектов.

В этом случае каталог `external/<libname>` содержит файл `CMakeLists.txt` с вызовом функций `FetchContent_Declare` и `FetchContent_MakeAvailable`.

### Git Submodules

[Подмодули](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

В этом случае вы добавляете репозиторий как подмодуль в каталог `external/<libname>`.

### Установка инструментов LLVM
В Ubuntu 22.04 версия 14 доступна из репозиториев:
```
sudo apt install clang-tidy clang-format
```
Репозитории LLVM и инструкции по установке доступны [здесь](https://apt.llvm.org/)

Пример для Ubuntu 20.04:
```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-14 main"
sudo apt install clang-tidy-14 clang-format-14
```
Есть известная [проблема с конфликтом зависимостей LLVM и Steam](https://steamcommunity.com/app/221410/discussions/0/2288338908683011755/), на других версиях не проверялось. Может приводить к сносу половины системы вместе с графическим интерфейсом. Читайте внимательно, на установку/удаление каких пакетов соглашаетесь.

В самом крайнем случае можно использовать версию не ниже 10, если она доступна в родных репозиториях вашего дистрибутива.

### Руководство

Сборку приложения следует выполнять в режиме out-of-source. См. [CMake FAQ](https://gitlab.kitware.com/cmake/community/-/wikis/FAQ).
1. Создайте репозиторий для проекта: https://classroom.github.com/a/9CFYd8ny
2. В репозитории создайте ветку `lab-1`. Как обычно, все работы выполняются в отдельных ветках.
3. Создайте структуру каталогов в соответствии со своим вариантом.
4. Создайте конфиги `.clang-format` и `.clang-tidy`.
5. Создайте библиотеку `fts`, содержащую функцию сложения двух чисел с плавающей точкой. Для настройки общих параметров библиотеки и всех прочих таргетов можете воспользоваться функцией (сохраните ее в `cmake/CompileOptions.cmake`):
```
function(set_compile_options target_name)
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /W4 /WX)
  else()
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror -pedantic)
  endif()

  set_target_properties(
    ${target_name}
    PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )

  if(CLANG_TIDY_EXE)
    set_target_properties(
      ${target_name}
      PROPERTIES
        CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
    )
  endif()
endfunction()
```
6. Разработайте приложение, использующее библиотеку. Пусть приложение принимает два числа через аргументы командной строки (для обработки аргументов командной строки используйте стороннюю библиотеку в соответствии со своим вариантом.):
```
$ ./sum --first 40 --second 2
42
```
7. Напишите один юнит-тест на функцию библиотеки. Для написания тестов используйте библиотеку [GoogleTest](https://github.com/google/googletest)
8. В CMake настройте запуск clang-tidy при компиляции. Убедитесь, что проверки срабатывают. Для определения переменной `CXX_CLANG_TIDY` воспользуйтесь функцией [find_program](https://cmake.org/cmake/help/latest/command/find_program.html)
9. Напишите [CMakePresets.json](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html). Предусмотрите конфигурации debug и release.
10. По готовности передайте выполненное задание на ревью преподавателю практики.

### Примеры
Разобранные на лекции исходники доступны здесь: [csc-cpp/cpp-examples/01-cmake](https://github.com/csc-cpp/cpp-examples/tree/main/01-cmake).

В примерах нет запуска тестов, их настройка остается частью самостоятельной работы.

### Контрольные вопросы
1. Опишите общую схему работы cmake.
2. Что такое in-source build и out-of-source build?
3. Расскажите о преимуществах и недостатках вашего способа использования сторонних библиотек. Предложите свой вариант решения этой задачи.

## 2. Парсер текста
### Цель
Реализовать парсер, который будет использоваться для нормализации поисковых запросов и содержимого документов. Покрыть тестами.
### Инструменты
Для реализации рекомендуется использовать средства стандартной библиотеки C++:
1. [Strings Library](https://en.cppreference.com/w/cpp/string)
2. [Algorithms library](https://en.cppreference.com/w/cpp/algorithm)
3. [Dynamic memory management](https://en.cppreference.com/w/cpp/memory)

Если вы реализуете алгоритм вручную, то это решение должно быть аргументированно.

При проверке работы учитываются:
1. Корректность алгоритма и качество тестов.
2. Идиоматичное использование C++.
3. Оптимальность (выбор структур данных, отсутствие избыточных аллокаций).

### Алгоритм работы парсера
Вход:
1. Строка для разбора.
2. Параметры парсера:
    1. Множество стоп-слов.
    2. Минимальная и максимальная длина n-граммы.

Выход: разобранный запрос, состоящий из списка n-грам и позиций n-gram в тексте. Позиция определяется как индекс токена, не считая индексов стоп-слов.

Примерная последовательность преобразований входной строки:
1. Удалить знаки пунктуации. Для проверки можно использовать функцию [std::ispunct](https://en.cppreference.com/w/cpp/string/byte/ispunct).
2. Привести символы к нижнему регистру ([std::tolower](https://en.cppreference.com/w/cpp/string/byte/tolower)).
3. Разделить строку на слова по пробельным символам ([std::isspace](https://en.cppreference.com/w/cpp/string/byte/isspace)).
4. Удалить из получившегося списка слов стоп-слова.
5. Для каждого слова сгенерировать список n-грамм длиной от `min_ngram_length` до `max_ngram_length`.

Стоп-слова — это слова, не влияющие на релевантность. Для первой итерации можете воспользоваться этим списком:
```
a
an
and
are
as
at
be
but
by
for
if
in
into
is
it
no
not
of
on
or
s
such
t
that
the
their
then
there
these
they
this
to
was
will
with
```
### Пример
Вход:
```
text: "Dr. Jekyll and Mr. Hyde"
stop_words: ["and", "dr", "mr"]
ngram_min_length: 3
ngram_max_length: 6
```
Выход:
```
jek 0 jeky 0 jekyl 0 jekyll 0 hyd 1 hyde 1
```

### Конфиг
Система поиска может содержать множество входных параметров. Передавать все через аргументы командной строки может быть слишком накладно, поэтому имеет смысл предусмотреть конфигурационный файл в машиночитаемом формате: xml, json или yaml. Для парсинга конфига вы можете использовать любую стороннюю библиотеку, например:
1. XML: [pugixml](https://pugixml.org/)
2. JSON: [nlohmann::json](https://github.com/nlohmann/json), [rapidjson](https://rapidjson.org/)
3. YAML: [yaml-cpp](https://github.com/jbeder/yaml-cpp)

Возможный вариант xml-конфига:
```
<?xml version="1.0" encoding="utf-8"?>
<fts>
   <parser ngram_min_length="3" ngram_max_length="6">
      <stop_words>
         <word>a</word>
         <word>an</word>
         …
      </stop_words>
   </parser>
</fts>
```
Путь к конфигурационному файлу удобно передавать через именованный аргумент командной строки, плюс предусмотреть ему значение по умолчанию:
```
--config <path> (default=config.xml)
```

### Дополнительно
Для разных языков и наборов данных оптимальные параметры парсера могут отличаться. Уже на этом этапе имеет смысл предусмотреть формат конфигурационного файла для всей поисковой системы.

## 3. Индексатор
### Цель
Реализовать построитель прямого и обратного индекса. Покрыть тестами.

### Основные понятия
Терм — это нормализованное слово, помещаемое в словарь поисковой системы. В нашем случае термами мы будем считать n-граммы, полученные от парсера.

В основе любой поисковой системы лежит обратный индекс. Это ассоциативная структура данных, ключами в которой являются термы, а значениями — список идентификаторов документов и позиций термов в этих документах.

На входе у индексатора документы, содержащие слова. Индексатор «разворачивает» связь `Документ -> [слова]` и строит структуру `терм -> [документы]`.

Прямой индекс — ассоциативная структура данных, в которой ключ — идентификатор документа, значение — сам документ.

### Пример
Пусть на входе у индексатора следующие документы:
```
    id  text
199903  The Matrix
200305  The Matrix Reloaded
200311  The Matrix Revolutions
```
От парсера индексатор получает следующие термы и их позиции:
```
    id  text
199903  mat:0 matr:0 matri:0 matrix:0
200305  mat:0 matr:0 matri:0 matrix:0 rel:1 relo:1 reloa:1 reload:1
200311  mat:0 matr:0 matri:0 matrix:0 rev:1 revo:1 revol:1 revolu:1
```
Тогда обратный индекс можно схематично изобразить так:
```
term    entries

mat:    {199903: [0], 200305: [0], 200311: [0]}
matr:   {199903: [0], 200305: [0], 200311: [0]}
matri:  {199903: [0], 200305: [0], 200311: [0]}
matrix: {199903: [0], 200305: [0], 200311: [0]}

rel:    {200305: [1]}
relo:   {200305: [1]}
reloa:  {200305: [1]}
reload: {200305: [1]}

rev:    {200311: [1]}
revo:   {200311: [1]}
revol:  {200311: [1]}
revolu: {200311: [1]}
```

### Декомпозиция
Один из возможных вариантов декомпозиции:
1. `IndexBuilder` — stateful-класс, накапливающий данные для индекса по мере добавления документов.
2. `Index::docs` — прямой индекс.
3. `Index::entries` — обратный индекс.
4. Сериализация индекса вынесена в отдельный класс `TextIndexWriter` для упрощения тестирования.

### Формат индекса
Ниже приведен рекомендуемый формат индекса. Вы можете спроектировать свой.
```
index
|-- docs
|   |-- 101
|   |-- 102
|   `-- 103
`-- entries
    |-- 3adb7b
    |-- 9ea46a
    `-- c9a868
```
1. `index/docs` — прямой индекс.
2. `index/docs/<document_id>` — проиндексированный документ
3. `index/entries` — обратный индекс. Имена вложенных файлов — первые три байта хеша терма. В качестве хеша можно использовать md5 или sha256 (https://github.com/okdshin/PicoSHA2).

Формат файлов обратного индекса `index/entries/<term-hash>`:
```
<term> <doc_count> [<doc_id> <pos_count> [<pos> …] …]
...
```
Здесь:
1. `term` — текст терма.
2. `doc_count` — количество документов, в которые входит терм. За ним следуют списки позиций для каждого документа.
3. `doc_id` — идентификатор документа.
4. `pos_count` — количество вхождений терма term в документ `doc_id`
5. `pos` — номер токена, в который входит терм.

Для примера выше вхождение терма `matrix` будет выглядеть так:
```
matrix 3 199903 1 0 200305 1 0 200311 1 0
```

## 4. Поиск и ранжирование
### Цель
Реализовать алгоритм поиска документов по обратному индексу и простейшую функцию оценки релевантности документа.

### Руководство
На входе у поисковика:
1. Текст запроса.
2. Конфиг: параметры парсера и прочие настройки, если потребуется.
3. Объект для доступа к индексу (IndexAccessor).

IndexAccessor предоставляет следующие возможности:
1. Получение содержимого документа по его идентификатору.
2. Получение списка документов по терму.

### Алгоритм:
1. Распарсить текст запроса.
2. Для каждого терма запроса извлечь список документов, в которые он входит. На этом этапе уместно вычислить следующие величины:
    + term frequency — количество вхождений терма в документ.
    + document frequency — количество документов, в которых встречается терм.
3. Для каждого документа вычислить оценку по [формуле](https://ru.wikipedia.org/wiki/TF-IDF)
4. Отсортировать результаты по убыванию `score`. В качестве стабилизирующего фактора сортировки можно использовать id документа.

### Пример
Пусть в индексе хранятся следующие документы:
```
id   text
100  Hello World
101  Bye World
102  Hello Earth
```
Для компактности примера пропустим этап выделения n-грам и будем считать, что каждое слово является термом. Тогда обратный индекс выглядит так:
```
bye:   {101: [0]}
earth: {102: [1]}
hello: {100: [0], 102: [0]}
world: {100: [1], 101: [1]}
```
Поиск:
```
query: bye earth

parsed_query: ["bye", "earth"]

# Для каждого терма получаем список документов и считаем tf и df.
bye:
   doc_ids: [101]
   tf:
      101: 1
   df: 1

earth:
   doc_ids: [102]
   tf:
      102: 1
   df: 1

score("bye earth", 101)
   = tf-idf("bye", 101) + tf-idf("earth", 101)
   = 1 * ln(3 / 1)      + 0
   ≈ 1.098612
```
Нетрудно заметить, что для документа 102 оценка будет такой же. Тогда результирующая выдача примет вид:
```
 id      score   text
101   1.098612   Bye World
102   1.098612   Hello Earth
```
Рассмотрим другой запрос:
```
query: Hello World

parsed_query: ["hello", "world"]

hello:
   doc_ids: [100, 102]
   tf:
      100: 1
      102: 1
   df: 2

world:
   doc_ids: [100, 101]
   tf:
      100: 1
      101: 1
   df: 2

score("hello world", 100)
   = tf-idf("hello", 100) + tf-idf("world", 100)
   = 1 * ln(3 / 2)        + 1 * ln(3 / 2)
   ≈ 0.810930

score("hello world", 101)
   = tf-idf("hello", 101) + tf-idf("world", 101)
   = 0                    + 1 * ln(3 / 2)
   ≈ 0.405465

score("hello world", 102)
   = tf-idf("hello", 102) + tf-idf("world", 102)
   = 1 * ln(3 / 2)        + 0
   ≈ 0.405465
```
Выдача примет вид:
```
 id      score   text
100   0.810930   Hello World
101   0.405465   Bye World
102   0.405465   Hello Earth
```

### Дополнительно
Что произойдет, если индексация и поиск будут выполнены с разными настройками парсера? Предложите и реализуйте решение этой проблемы.

## 5. Поиск по базе книг
### Цель
Реализовать индексацию и поиск по данным [books.csv](https://csc-cpp.readthedocs.io/ru/2022/_static/s1/books.csv). В качестве документов можно использовать только названия книг.

### Руководство
Если вы используете библиотеку CLI11, то можете реализовать одну консольную утилиту с командами:
```
fts index --csv <path> --index <path>
fts search --index <path> [--query <query_text>]
```
Библиотека cxxopts не поддерживает синтаксиса команд, поэтому можно реализовать две отдельные утилиты:
```
indexer --csv <path> --index <path>
searcher --index <path> [--query <query_text>]
```
Поисковик при запуске без параметра `--query` переходит в интерактивный режим и ожидает ввод запроса. Для реализации такого режима можете воспользоваться библиотекой [AmokHuginnsson/replxx](https://github.com/AmokHuginnsson/replxx).

CSV-файл можно парсить вручную или выбрать одну из существующих библиотек:
1. [awesome-cpp#csv](https://github.com/fffaraz/awesome-cpp#csv)
2. [rapidcsv](https://github.com/d99kris/rapidcsv)
3. [GitHub: csv-parser](https://github.com/topics/csv-parser?l=c%2B%2B&o=desc&s=stars)

Скорее всего, вы заметите, что поисковая выдача содержит множество нерелевантных результатов. Предложите и реализуйте механизм отсечения мусора.

### Дополнительно
1. Предложите способ реализации поиска по нескольким полям: заголовку и автору. Какие компоненты поисковой системы затронет такое изменение?
2. Насколько хорошо ваш парсер справляется с нормализацией слов? Какие вы можете найти слабые места? Выполните необходимые доработки.
3. Предложите способ реализации поиска по ISBN и ISBN13. Чем он отличается от поиска по текстовым полям?
4. Ручное тестирование качества поиска — крайне трудоемкая задача. Сгенерируйте корпус запросов и предложите критерии для оценки качества поиска. Для автоматизации интеграционного тестирования может буть удобно использовать более высокоуровневый язык программирования.

## 6. FFI
### Постановка задачи
Реализуйте обертку на языке C поверх поисковых функций библиотеки `libfts`. Напишите клиент на языке программирования, отличном от C и C++. Использование библиотеки должно выглядеть максимально нативно для выбранного вами языка. Инструменты для вызова C-функций из других языков:
1. Python: [ctypes](https://docs.python.org/3/library/ctypes.html)
2. PHP: [ffi](https://www.php.net/manual/en/class.ffi.php)
3. Go: [cgo](https://pkg.go.dev/cmd/cgo)
4. JavaScript: [node-ffi](https://github.com/node-ffi/node-ffi), [node-ffi-napi](https://github.com/node-ffi-napi/node-ffi-napi)

Пример: https://github.com/csc-cpp/cpp-examples/tree/main/11-ffi/07-py-to-cpp-opaque

## 7. Бинарный индекс
### Цель
В ЛР3 вы реализовали хранение текстового индекса. Такой формат прост для отладки, но неоптимален во всех отношениях: для файла books.csv размером 1.5 Мб текстовый индекс занимает более 100 Мб.

Цель этой работы — реализовать хранение индекса в бинарном формате. Если в предыдущих ЛР вы действовали достаточно аккуратно, то код поиска останется практически без изменений.

### Формат индекса
Для описания отдельных полей индекса будем использовать записи вида:
```
<name:type>
```
Где:
1. name — имя поля. Нигде не хранится, используется только в целях документации.
2. type — тип поля. Будем использовать сокращения: u32 — std::uint32_t, i16 — std::int16_t и т. д.

Для хранения строк будем использовать тип string:
```
string = <length:u8><data:char[]>
```
Строки хранятся без символа „0“. Таким образом, строка «hello» будет храниться как следующая последовательность:
```
06 68 65 6c 6c 6f
```
Где первый байт 06 — длина строки, а последующие — коды символов.

Числа размером более одного байта хранятся в little endian.

### Секции
Индекс состоит минимум из четырех секций:
```
<header>
<dictionary>
<entries>
<docs>
```
Они хранятся в том же порядке, в котором будут использованы при поиске, однако строить их удобнее в обратном порядке. Опишем форматы секций в порядке их построения.

### Секция docs
Прямой индекс. Представляет собой список строк, в нашем случае — список заголовков книг. Формат:
```
<titles_count:u32>[<string>...]
```
На этапе построения нужно запомнить отображение: `DocumentId -> DocumentOffset`, где DocumentOffset — смещение заголовка от начала секции docs. Это смещение будет использоваться в обратном индексе для идентификации документа.

### Секция entries
Обратный индекс. Список структур TermInfo, аналогичных содержимому файлов в каталоге `entries` индекса в текстовом формате. Формат:
```
[<doc_count:u32>
    [
        <doc_offset:u32>
        <pos_count:u32>
        [<pos:u32> …]
        …
    ]
 …
]
```
Единственное отличие от текстового индекса: вместо идентификатора документа мы используем `doc_offset` — смещение, которое запомнили на этапе записи секции docs.

На этапе построения нужно запомнить смещение каждого TermInfo.

### Секция dictionary
Словарь. Хранится как префиксное дерево в порядке обхода в глубину. Формат:
```
<children_count:u32>
[<letter:u8> …]
[<child_offset:u32> …]
<is_leaf:u8>
<entry_offset:u32>?
```
Где:
1. children_count — количество дочерних узлов текущего узла.
2. [<letter:u8>] — массив букв дочерних узлов
3. [<child_offset:u32>] — массив смещений дочерних узлов от начала секции dictionary
4. is_leaf — 1, если узел является последним символом в слове, 0 — для промежуточного.
5. entry_offset — смещение структуры TermInfo для текущего entry. Присутствует только для is_leaf = 1.

### Секция header
Заголовок содержит список пар «имя секции» + смещение секции от начала индекса.
Формат:
```
<section_count:u8>
[<section_name:string><section_offset:u32>]×section_count
```

### Инициализация аксессора
1. Сначала нужно получить байтовый массив с содержимым индекса. Можно вычитать весь файл в `std::vector<char>` или (лучше) отмапить его функцией mmap.
2. Вычитать секцию Header. Получим доступ к смещениям каждой секции.
3. Сконструировать аксессор. Для этого нужно передать ему данные индекса и Header. Из заголовка можно получить смещения каждой секции и инициализировать аксессоры каждой секции.

### Паника-паника, с чего начать?
С простого. Секция docs самая простая по своей структуре, начните с нее. Напишите вспомогательный класс `BinaryBuffer` для записи бинарных данных, сериализуйте массив заголовков книг в указанном выше формате, запишите его в файл. Секция достаточно проста, и вы легко сможете проверить корректность данных в hex-редакторе.

# Лабораторные работы
## 1. Перегрузка операторов
### Цель
1. Повторить основы перегрузки операторов.
2. Реализовать утилитарные классы для работы с временными интервалами.
```
cpp-labs
|-- src/libcsc/libcsc/time
|   |-- time.cpp (опционально)
|   `-- time.hpp
`-- test/libcsc/libcsc
    `-- time.cpp
```
### Ход работы
Создайте репозиторий для ЛР  
Реализуйте классы:
1. Time — момент времени.
2. TimeSpan — интервал.  

Перегрузите операторы для реализации следующих операций:
1. Time - Time = TimeSpan
2. Time + TimeSpan = Time
3. Time - TimeSpan = Time
4. TimeSpan + TimeSpan = TimeSpan
5. TimeSpan - TimeSpan = TimeSpan  

Перегрузите оператор << для вывода в формате: `XXd XXh XXm XXs` (пример: `01d 23h 10m 11s`).  
Реализуйте пользовательские литералы в пространестве имен `<your_namespace>::literals`: `_d`, `_h`, `_m`, `_s`.  
Перегрузите операторы сравнения. Если вы используете С++17, то операторы `==`, `!=`, `<`, `<=`, `>`, `>=`. Для С++20 достаточно определить оператор `<=>`.

### Ссылки
1. [Affine Space Types](http://videocortex.io/2018/Affine-Space-Types/)
2. [User-defined literals](https://en.cppreference.com/w/cpp/language/user_literal)

## 2. Контейнеры и алгоритмы STL
### Цель
1. Получить базовые навыки использования алгоритмов и контейнеров стандартной библиотеки C++.
2. Реализовать и покрыть тестами алгоритм Soundex.
3. Реализовать утилиту для группировки фонетически близких имен с выводом в машиночитаемом формате (json или xml).  
```
cpp-labs
|-- src/libcsc/libcsc/soundex
|   |-- soundex.cpp
|   `-- soundex.hpp
|-- src/soundex_groupby/soundex_groupby
|   `-- main.cpp
`-- test/libcsc/libcsc
    `-- soundex.cpp
```
### Ход работы
#### Реализация soundex
Реализуйте функцию для вычисления фонетического хеша строки:
```
uint32_t soundex_hash(std::string_view str);
```
При реализации алгоритма не пишите циклы, по максимуму пользуйтесь стандартными алгоритмами: [Algorithms library](https://en.cppreference.com/w/cpp/algorithm).  
В этой части ограничтесь unconstrained версиями алгоритмов, т.е. из пространства имен std, а не std::ranges.  
Алгоритм:
1. Запоминается первая буква слова.
2. Удаляются все вхождения h и w (за исключением первой буквы слова).
3. Согласные заменяются на цифры от 1 до 6, причём похожим по звучанию буквам соответствуют одинаковые цифры:
```
b, f, p, v → 1
c, g, j, k, q, s, x, z → 2
d, t → 3
l → 4
m, n → 5
r → 6
```
4. Любая последовательность одинаковых цифр сокращается до одной такой цифры.
5. Удаляются все a, e, i, o, u, y (за исключением первой буквы слова).
6. Заменяется первый символ буквой, запомненной на шаге 1, делая её заглавной.
7. Итоговая строка обрезается до первых четырёх символов. Если длина строки меньше требуемой, недостающие символы заменяются знаком 0.  

#### Покрытие тестами
Все как обычно: классы эквивалентности, граничные значения.
#### Обработка файла
Напишите утилиту, которая:
1. Парсит csv-файл [names2020.csv](https://csc-cpp.readthedocs.io/ru/2022/_static/s2/names2020.csv).
2. Группирует имена из файла по их soundex-хешам.
3. Выводит результат в машиночитаемом формате в соответствии со своим вариантом. Вывод должен быть стабильным (Хеши должны быть отсортированы по возрастанию и имена внутри каждого списка также должны быть отсортированы).  

Способ ввода и вывода данных (stdin/stdout/файлы) остается на ваше усмотрение, но выбор должен быть аргументирован. Считаем, что утилита для продакшена, хардкод не допускается.  
Формат входного файла:
```
<name>,<gender>,<count>
```
Можете воспользоваться любой библиотекой для парсинга csv или с помощью стандартных алгоритмов отделить первое поле.  
Варианты вывода:
1. json. Библиотека: [nlohmann/json](https://github.com/nlohmann/json)
2. xml. Библиотека: [zeux/pugixml](https://github.com/zeux/pugixml)

## 3. Умные указатели
```
cpp-labs
|-- src/libcsc/libcsc/smartptr
|   |-- SharedPtr.hpp
|   `-- UniquePtr.hpp
`-- test/libcsc/libcsc
    `-- smartptr.cpp
```
Реализуйте два шаблонных типа умных указателей:
1. UniquePtr - тип без счетчика ссылок, безраздельно владеющий указателем.
2. SharedPtr - тип со счетчиком ссылок для разделяемого владения указателем.  

Реализуйте специальные методы в соответствии с [Rule of five](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all).  
Реализуйте следующие методы классов. В сигнатурах намеренно не указана константность, определите самостоятельно, в каких случаях она нужна.  
Для UniquePtr и SharedPtr:
1. T &operator*()
2. T *operator->()
3. T *get()
4. void reset(T *p = nullptr)  

Только для SharedPtr: std::size_t use_count()  
Покройте тестами, убедитесь в отсутствии утечек памяти.

## 4. Итераторы
Реализуйте односвязный список любым простым способом.  
Реализуйте возможность создавать списки с циклами.  
Реализуйте итератор и функцию проверки наличия цикла в списке.  
Техника проверки:
1. Создать два итератора: один перемещается по списку с шагом 1 (черепаха), другой с шагом 2 (заяц).
2. Одновременно обойти список двумя итераторами.
3. Если оба итератора дошли до конца списка, то цикла нет.
4. Если в процессе обхода итераторы оказались на одном элементе, то цикл есть.

## Дополнительно: частотный словарь
Реализуйте программу для построения частотного словаря на трех языках:
1. C++
2. Java
3. PHP

Измерьте время построения словаря. Сравните. Оптимизируйте.
### Ход работы
1. В качестве данных используйте файл [words-5m.txt](https://csc-cpp.readthedocs.io/ru/2022/_static/s2/words-5m.txt).
```
$ wget https://csc-cpp.readthedocs.io/ru/2022/_static/s2/words-5m.txt
```
2. Прочитайте слова из файла в последовательный контейнер.
3. Для каждого слово вычислите количество его появлений. Воспользуйтесь подходящей структурой данных.
4. Измерьте время подсчета количества слов.
5. Для самопроверки можете вывести общее количество уникальных слов:
```
$ sort words-5m.txt | uniq --count | wc --lines
58410
```
и топ-10 частотных слов:
```
$ sort words-5m.txt | uniq --count | sort --human --reverse | head --lines 10
 364031 the
 211763 of
 154576 and
 132248 to
 115603 in
 105114 a
  57269 is
  54994 that
  44676 for
  39406 it
```

## РГР
### Цель
Реализовать шаблон класса для структуры данных HashMap. Реализовать итераторы, совместимые с алгоритмами стандартной библиотеки. Покрыть тестами.  
Считаем, что вы полностью реализуете управление ресурсами, поэтому внутри классов не используйте контейнеры стандартной библиотеки.  
Начните с проектирования интерфейса: какие операции должна поддерживать хеш-таблица.
