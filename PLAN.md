# code-100 — Expansion Plan

## Цель

Добавить новые челленджи для прокачки навыков software engineering:
архитектура, системное программирование, идиомы разных языков.

## Языки (на выбор)

| Язык       | Фокус                                         |
|------------|-----------------------------------------------|
| C          | ручное управление памятью, низкий уровень     |
| Python     | быстрая разработка, богатая stdlib            |
| Go         | встроенная конкурентность, io, net            |
| Rust       | zero-cost abstractions, memory safety         |
| TypeScript | async/await, event loop, Node.js экосистема   |

## Челленджи

| #  | Название                        | Архитектурные навыки                                        |
|----|---------------------------------|-------------------------------------------------------------|
| 21 | JSON Parser                     | recursive descent, структуры данных, обработка ошибок       |
| 22 | Key-Value Store                 | storage engine, B-tree/hash index, file I/O, кэширование   |
| 23 | HTTP Server                     | TCP networking, разбор протокола, routing, middleware       |
| 24 | Chat Server + Client            | конкурентность, протокол, async I/O                        |
| 25 | Template Engine                 | парсинг, DSL, строки, escaping                              |
| 26 | Mini Test Framework             | discovery, регистрация, assertions, отчёты                  |
| 27 | Brainfuck Interpreter           | токенизация, AST/bytecode, исполнение, REPL                |
| 28 | Static Site Generator           | file watching, markdown→HTML, asset pipeline               |
| 29 | TUI File Manager                | ncurses, event loop, async файловые операции                |
| 30 | Task Runner / Make-lite         | граф зависимостей, запуск процессов, параллелизм            |

## Формат

Каждый челлендж — директория `NN-name/` с файлами:
- `NN-name.c`
- `NN-name.py`
- `NN-name.go`
- `NN-name.rs`
- `NN-name.ts`
- `test.exp` (expect-скрипты для тестов)

Общий код (shared utilities) — в `common/`.

## Очередность

1. Утвердить языки (первые 2–4)
2. Реализовать первые 3–4 челленджа
3. Добавить тесты (`test.exp`)
4. Постепенно расширять
