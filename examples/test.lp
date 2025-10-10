(* "std.lp")


(@ FORMULA  \a, b. (IFF (IFF a, b), (NOT (XOR a, b))))

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

([$][-]& (CHECK_TOTOLOGY FORMULA))
