
(@ I \x. x)

(@ TRUE \x y. x)
(@ FALSE \x y. y)

(@ NOT \b, x, y. (b y, x))
(@ AND \a, b. (a b, a))
(@ OR  \a, b. (a a, b))
(@[$] XOR  \a, b. (a (NOT b), b))
(@[$] IMPL \a, b. (a b, (NOT a)))
(@[$] IFF  \a, b. (a b, (NOT b)))

(@ SUCC \n f x. (f, (n f x)))

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
