


%
	\lprogSTD: Chruch Arithmetic

	REQUIRES: Boolean, Definitions.

	Basic functions for doing arithmetic using the Church representation.
%


(@ SUCC \n, f, x. (f, (n f x)))
(@ PREC \n, f, x. (n (\g, h. (h (g f))), \u. x, \u. u))

(@ ZERO \f, x. x)
(@[$] ONE   (SUCC ZERO))
(@[$] TWO   (SUCC ONE))
(@[$] THREE (SUCC TWO))
(@[$] FOUR  (SUCC THREE))
(@[$] FIVE  (SUCC FOUR))
(@[$] SIX   (SUCC FIVE))
(@[$] SEVEN (SUCC SIX))
(@[$] EIGHT (SUCC SEVEN))
(@[$] NINE  (SUCC EIGHT))
(@[$] TEN   (SUCC NINE))

(@[$] IS_ZERO \n. (n (\x. FALSE), TRUE))

(@ ADD    \n, m, f, x. (n f, (m f x)))
(@[$] SUB \n, m. (m PREC n))
(@ MUL    \n, m, f. (n (m f)))
(@[$] POW    \n, m. (IF_THEN_ELSE (IS_ZERO m) ONE (m, n)))

(@[$] LEQ \n, m. (IS_ZERO (SUB n, m)))
(@[$] GEQ \n, m. (IS_ZERO (SUB m, n)))
(@[$] LST \n, m. (AND (LEQ n, m), (NOT (GEQ n, m))))
(@[$] GST \n, m. (AND (GEQ n, m), (NOT (LEQ n, m))))
(@[$] EQ  \n, m. (AND (LEQ n, m), (GEQ n, m)))


(DEF_REC DIV \s, n, m.
	(IF_THEN_ELSE (LEQ n, (PREC m))
		ZERO,
		(SUCC (REC s, (SUB n, m), m))
	)
)


(DEF_REC MOD \s, n, m.
	(IF_THEN_ELSE (LEQ n, (PREC m))
		n,
		(REC s, (SUB n, m), m)
	)
)

(DEF_REC FAC \s, n.
	(IF_THEN_ELSE (IS_ZERO n),
		ONE,
		(MUL n, (REC s, (PREC n)))
	)
)

% Maybe should put this somewhere else... %

(DEF_REC FIB \s, n.
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
