
(@ I \x. x)
(@ SELF \x. (x, x))

(@ TRUE \x y. x)
(@ FALSE \x y. y)

(@ NOT \b, x, y. (b y, x))
(@ AND \a, b. (a b, a))
(@ OR  \a, b. (a a, b))
(@[$] IMPL \a, b. (a b, (NOT a)))
(@[$] XOR  \a, b. (a (NOT b), b))
(@[$] IFF  \a, b. (a b, (NOT b)))

(@ IF_THEN_ELSE \p, a, b. (p a, b))

(@ SUCC \n, f, x. (f, (n f x)))
(@ PREC \n, f, x. (n (\g, h. (h (g f))), \u. x, \u. u))

(@ ADD    \n, m, f, x. (n f, (m f x)))
(@[$] SUB \n, m. (m PREC n))
(@ MUL    \n, m, f. (n (m f)))
(@ POW    \n, m. (m, n))

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

(@[$] LEQ \n, m. (IS_ZERO (SUB n, m)))
(@[$] GEQ \n, m. (IS_ZERO (SUB m, n)))
(@[$] EQ  \n, m. (AND (LEQ n, m), (GEQ n, m)))


(@ REC \func, arg. (SELF func, arg))
($! (@ DEF_DISC \name, func. ($! (@ name, func))))
(DEF_DISC DEF_REC \name, func. (DEF_DISC name (SELF, func)))

(DEF_REC OMEGA SELF)
(DEF_DISC Y \f. (SELF, (\x. (f (x, x)))))
