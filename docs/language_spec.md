# Specification for the Lambda prog (\lprog) language

!!! INSANELY OUTDATED !!!

## Elements

### In-language elements

As inspired by the lambda calculus theory, the `\lprog` language is made to be extremely simple. Henceforth in the language itself there is only 3 components:

- **Variables**: Denoted by any string of _lowercase_ ASCII letters (`a-z`).
- **Functions**: Denoted by `\x, y, [...] . (...)`.
- **Applications**: Denoted by `((...), (...), [...])`

### Macros

- `&`: Display: Returns previous
- `#`: Tree: Returns previous
- `$`: Eval: Returns previous evaluated
- `% NAME [(...) (...), [...]]`: Macro Def.: Returns Nothing
- `@ NAME (...)`: Code Macro Def.: Returns Nothing

- `*`: File Include: Returns Nothing;


### Others

- `"(...)"`: String (right now only used in the file include part)
- `//`: Line Comment
- `/* (...) */` Block Comment


## Syntax

Be aware that spaces (or new lines / tabs) are supposed to be _NEVER_ taken into account by the parser (except for line comments).
