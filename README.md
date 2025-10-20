# FunctionalSimulator

Простой функциональный симулятор написанный на C++ и Ruby

## Сборка проекта

### Требования
- CMake 3.14
- C++ компилятор с поддержкой C++20

### Сборка

```bash
# Клонирование репозитория
git clone https://github.com/OlejaCat/FunctionalSimulator.git
cd FunctionalSimulator
# Сборка
cmake -B build -S .
cmake --build build
```

## Запуск симулятора

```bash
./build/simulator
```

## Основные команды

| Команда | Алиас | Описание |
|---------|-------|----------|
| `set_register` | `sr` | Установить значение регистра |
| `set_pc` | `sp` | Установить program counter |
| `run_cycle` | - | Выполнить один цикл конвейера |
| `run_program` | - | Выполнить программу до завершения |
| `print_reg` | - | Показать все регистры |
| `load` | - | Загрузить программу из файла |
| `reset` | - | Сбросить все регистры и PC в 0 |
| `help` | - | Показать справку по командам |
| `exit` | - | Выйти из симулятора |

## Пример запуска для получения n-го числа Фибоначчи

```bash
# Загружаем в память программу 
> load
examples/fib.bin
Program loaded: 20 bytes

# Установка всех нужных для работы регистров
> sr
2 1
R2 = 1

> sr
3 5
R3 = 5

> sr
5 -1
R5 = -1

# Запуск программы
> run_program
Program executed.

# Выводим значения регистров
> print_reg
PC: 0x18
R0: 0x0
R1: 0x5 # пятое число фибоначчи 
R2: 0x8 # шестое число фибоначчи
R3: 0x0
R4: 0x8
R5: 0xffffffff
```


