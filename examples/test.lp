(* "std.lp")


%(@[$] FORMULA \a, b. (IFF (IFF a, b), (NOT (XOR a, b))))

(@ CHECK_TOTOLOGY \formula. (AND
	(AND
		(formula TRUE TRUE),
		(formula TRUE FALSE)
	)
	(AND
		(formula FALSE TRUE),
		(formula FALSE FALSE)
	)
))

([$][-]# (CHECK_TOTOLOGY FORMULA))

([$][-]& (PLUS FOUR THREE))
([$][-]& (MINUS SIX, THREE))
([#$?][-]& (MULT THREE, TWO))%

(DEF_REC FACTORIAL \s, n.
	(IF_THEN_ELSE (IS_ZERO n),
		ONE,
		(MUL n, (REC s, (PREC n)))
	)
)

(DEF_REC FIBONACCI \s, n.
	(IF_THEN_ELSE (IS_ZERO n),
		ZERO,
		(IF_THEN_ELSE (IS_ZERO (PREC n)),
			ONE,
			(ADD
				(REC s, (PREC n)),
				(REC s, (SUB n, TWO))
			)
		)
	)
)

([#$][-]& (ADD TEN TEN))
