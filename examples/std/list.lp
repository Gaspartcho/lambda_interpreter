


%
	\lprogSTD: Lists

	Basic functions to work with lists.
%

(@ EMPTY_LIST \f. f)

(DEF_REC HALF_SELECTOR \s, n, x.
	(IF_THEN_ELSE (IS_ZERO n)
		x
		\y. (REC s, (PREC n), x)
	)
)

(DEF_DIS SELECTOR \n, size. ((HALF_SELECTOR (PREC n)) (HALF_SELECTOR (SUB size, n))))

(@ APPEND \list, elem. \f. (list, f, elem))
(@ FLUSH \list. \f. (list \x. f))
(@ CONCAT \list_one, list_two. \f. (list_one (list_two f)))

(@ ARRAY \size, list. \f. (f size, list))
(@[$] EMPTY_ARRAY \f. (f ZERO, EMPTY_LIST))
(@ GET_ARRAY_LIST \array. (array, \size, list. list))
(@ GET_ARRAY_SIZE \array. (array, \size, list. size))

(DEF_DIS ARRAY_AT \array, n. ((GET_ARRAY_LIST array) (SELECTOR n, (GET_ARRAY_SIZE array))))

(@[$] APPEND_ARRAY \array, elem.
	(ARRAY
		(SUCC (GET_ARRAY_SIZE array)),
		(APPEND (GET_ARRAY_LIST array) elem)
	)
)

(@[$] FLUSH_ARRAY \array.
	(ARRAY
		(PREC (GET_ARRAY_SIZE array)),
		(FLUSH (GET_ARRAY_LIST array))
	)
)
