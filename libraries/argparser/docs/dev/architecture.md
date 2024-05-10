# Архитектура продукта

В этом документе описывается архитектура продукта - парсера аргументов командной 
строки, разработанная на основании [требований](requirements.md).

## Системная архитектура

* Продукт не имеет системной архитектуры, поскольку его предполагается использовать 
  в качестве подсистемы, встраиваемой в иные продукты.

#### UML-диаграмма
```mermaid
---
title: Diagram of the system of the project
---
flowchart TB
    nodeArgParser([ArgParser])
```

## Архитектура подсистем

* Исходя из вышеуказанного предполагаемого использования продукта, весь продукт 
  представляет собой одну подсистему - библиотеку argparer, которая должна обрабатывать
  данные аргументы командной строки.

### Архитектура подсистемы "ArgParser"

#### UML-диаграмма
```mermaid
---
title: Diagram of the module ArgParser
---
%%{
  init: {
    'theme': 'base',
    'classDiagram': { 'curve': 'linear' },
  }
}%%
classDiagram
    direction TB
    note for ArgParser "Has pseudonym functions for AddArgument and GetValue for each argument type"
    class ArgParser {
        -const char[] name_;
        -vector~ArgumentBuilder*~ argument_builders_;
        -vector~Argument*~ arguments_;
        -vector~string~ allowed_typenames_;
        -vector~string~ allowed_typenames_for_help_;
        -map~string, map~ string, size_t~~ arguments_by_type_;
        -map~char, string~ short_to_long_names_;
        -size_t help_index_;
        +Parse(vector~string~ args, ConditionalOutput error_output=()) bool
        +Parse(int argc, char[][] argv, ConditionalOutput error_output=()) bool
        +Help() bool
        +HelpDescription() string
        +AddHelp(char short_name, const char[] long_name, const char[] description="") ConcreteArgumentBuilder~bool~ &
        +AddHelp(const char[] long_name, const char[] description="") ConcreteArgumentBuilder~bool~ &
        +AddArgument~T~(char short_name, const char[] long_name, const char[] description="") ConcreteArgumentBuilder~T~ &
        +AddArgument~T~(const char[] long_name, const char[] description="") ConcreteArgumentBuilder~T~ &
        +GetValue~T~(const char[] long_name, size_t index=0) T
        -Parse_(vector~string~ args, ConditionalOutput error_output) bool
        -GetLongKeys(string current_argument) vector~string~
        -ParsePositionalArguments(vector~string~ argv, const vector~size_t~ & used_positions) void
        -HandleErrors(ConditionalOutput error_output) bool
        -RefreshArguments() void
        -AddArgument_~T~(char short_name, const char[] long_name, const char[] description) ConcreteArgumentBuilder~T~ &
        -GetValue_~T~(const char* long_name, size_t index) T
    }
    class Argument {
        <<interface>>
        +ValidateArgument(vector~string~ argv, size_t position)* vector~size_t~
        +CheckLimit()* bool
        +GetValueStatus()* ArgumentParsingStatus
        +GetType()* string
        +GetInfo()* ArgumentInformation
        +GetUsedValues()* size_t
        +ClearStored()* void
        #ObtainValue(vector~string~ argv, string& value_string, vector~size_t~ & used_values, size_t position)* size_t
    }
    class ArgumentBuilder {
        <<interface>>
        +GetInfo()* ArgumentInformation
        +GetDefaultValue()* string
        +build()* Argument*
    }
    class ConcreteArgument~T~ {
        -ArgumentInformation info_
        -ArgumentParsingStatus value_status_
        -size_t value_counter_
        -T value_
        -T default_value_
        -T* stored_value_
        -vector~T~* stored_values_
        +GetValue(size_t index) T
        +ValidateArgument(vector~string~ argv, size_t position) vector~size_t~
        +CheckLimit() bool
        +GetValueStatus() ArgumentParsingStatus
        +GetType() string
        +GetInfo() ArgumentInformation
        +GetUsedValues() size_t
        +ClearStored() void
        #ObtainValue(vector~string~ argv, string& value_string, vector~size_t~ & used_values, size_t position) size_t
    }
    class ConcreteArgumentBuilder~T~ {
        -ArgumentInformation info_;
        -T default_value_;
        -T* stored_value_;
        -vector~T~* stored_values_;
        -bool was_created_temp_vector_;
        +MultiValue(size_t min=0) ConcreteArgumentBuilder &
        +Positional() ConcreteArgumentBuilder&
        +StoreValue(T& value) ConcreteArgumentBuilder&
        +StoreValues(vector~T~ & values) ConcreteArgumentBuilder&
        +Default(T value) ConcreteArgumentBuilder&
        +AddValidate(function~bool(string&)~ validate) ConcreteArgumentBuilder&
        +AddIsGood(function~bool(string&)~ is_good) ConcreteArgumentBuilder&
        +GetInfo() ArgumentInformation
        +GetDefaultValue() string
        +build() Argument*`
    }
    class ArgumentInformation {
        +char short_key = kBadChar
        +const char[] long_key = ""
        +const char[] description = ""
        +string type
        +size_t minimum_values = 0
        +bool is_multi_value = false
        +bool is_positional = false
        +bool has_store_values = false
        +bool has_store_value = false
        +bool has_default = false
        +function~bool(string&)~ validate = &AlwaysTrue
        ++function~bool(string&)~ is_good = &AlwaysTrue
    }
    class ArgumentParsingStatus {
        <<enumeration>>
        NoArgument
        InvalidArgument
        InsufficientArguments
        Success
    }

    ArgParser *-- Argument
    ArgParser *-- ArgumentBuilder
    ArgParser <.. ConcreteArgument
    ArgParser <.. ConcreteArgumentBuilder
    Argument <|.. ConcreteArgument
    ArgumentBuilder <.. Argument
    ArgumentBuilder <|.. ConcreteArgumentBuilder
    ConcreteArgument *-- ArgumentInformation
    ConcreteArgument *-- ArgumentParsingStatus
    ConcreteArgumentBuilder <.. ConcreteArgument
    ConcreteArgumentBuilder *-- ArgumentInformation
```

Эта подсистема представляет собой набор классов и связей между ними, которые выполняют
непосредственно парсинг аргументов командной строки, передаваемых в подсистему. Все
классы находятся в пространстве имён ArgumentParser.

#### Класс ArgParser

Этот класс является основным классом модуля, именно с ним обычно взаимодействует 
пользователь. Он должен предоставлять следующие возможности: добавление обрабатываемого
аргумента (любого из указанных) с указанным ключом, парсинг набора аргументов командной
строки, а также функция добавления аргумента. Кроме того, должен иметь функцию 
составления справки и обрабатывать ошибки в синтаксисе аргументов командной строки,
и, в некоторых случаях, выводить их.

#### Класс ArgumentBuilder

Этот класс реализует паттерн проектирования "Builder": ссылки на объекты наследников 
этого класса должна возвращать функция добавления аргумента из ArgParser, к нему 
должен обращаться пользователь, добавляя информацию об аргументе. Должен 
реализовывать функции добавления любой информации про аргумент, представленные в 
[тестах](../../../../tests/argparser_unit_tests.cpp) и функцию построения. Ему должен 
наследовать шаблонизированный класс конкретного Builder, который будет реализовывать 
вышеуказанный функционал для каждого из типов аргумента.

#### Класс Argument

Этот класс является родительским классом для всех классов аргументов. Должен иметь
функции возврата статуса парсинга и возврата информации об аргументе, а также функцию 
получения значения аргумента из аргументов командной строки. Необходимая информация, 
не изменяемая в процессе парсинга, должна храниться в виде экземпляра структуры. 
На данный момент планируется аргументы всех базовых значащих типов (кроме 8-битных 
чисел), StringArgument и ComplexArgument (строка с валидацией и чтением пробелов).
