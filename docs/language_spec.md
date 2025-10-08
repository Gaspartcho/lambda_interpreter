# Specification for the Lambda prog (\lprog) language

This is the official \lprog language specification document

## Language Elements:


### Variables

Syntax:

```lp
variable
```

### Functions

Syntax:

```lp
\ variable. ANY
```



```lp
\a. \b. \c. ANY
\ a, b, c. ANY
```

### Applications

Syntax:

```lp
(ANY, ANY)
```

The following two lines are equivalent:

```lp
((F, G) H)
(F, G, H)
```

```lp
(ANY)
ANY
```

### Macros

Syntax:

```lp
MACRO
```

### Strings

Syntax:

```lp
"string"
```

### Directives

#### Display

Syntax:

```lp
(& ANY)
```

```

```

#### Tree

Displays the element given in argument as _Tromp Diagram_ (See [https://tromp.github.io/cl/diagrams.html](https://tromp.github.io/cl/diagrams.html))

Syntax:

```lp
(# ANY)
```

Tree representation for the number 6.
```
━┯━┯━┯━┯━┯━━━
━┿━┿━┿━┿━┿━┯━
 │ │ │ │ │─┘
 │ │ │ ├─┘
 │ │ ├─┘
 │ ├─┘
 ├─┘
```

#### Macro Definition

Syntax:

```lp
(@ MACRO ANY)
((@ MACRO) ANY)
```

Any directives placed after the macro definition symbol will be applied to the macro data (the `ANY` above)



#### Include

Syntax:

```lp
(* "path_to_file")
```

## Behaviors
