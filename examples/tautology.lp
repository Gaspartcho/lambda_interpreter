(* "std/std.lp")


(@[$] FORMULA \a, b, c, d. (IMPL a (IMPL b (IMPL c (IMPL d a)))))

(DEF_DIS GEN_BOOL \n, i. (LEQ (MOD i (POW TWO n)) (PREC (POW TWO (PREC n)))))

(DEF_REC GEN_BOOL_LIST \s, nb_vars, i.
	(IF_THEN_ELSE (IS_ZERO nb_vars)
		EMPTY_LIST,
		(APPEND
			(REC s, (PREC nb_vars), i)
			(GEN_BOOL nb_vars, i)
		)
	)
)

(@ APPLY_FORMULA \formula, bool_list. (bool_list, formula))

(DEF_REC TAUTOLOGY_REC \s, formula, nb_vars, i.
	(IF_THEN_ELSE (IS_ZERO i),
		TRUE,
		(AND
			(REC s, formula, nb_vars, (PREC i)),
			(APPLY_FORMULA formula, (GEN_BOOL_LIST nb_vars, i))
		)
	)
)

(DEF_DIS TAUTOLOGY \formula, nb_vars. (TAUTOLOGY_REC formula, nb_vars, (POW TWO nb_vars)))



([$]# (TAUTOLOGY FORMULA, FOUR))
