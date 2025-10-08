
(@ I \x. x)

(@ TRUE \x y. x)
(@ FALSE \x y. y)

(@ NOT \a. (a FALSE, TRUE))
(@ AND \a, b. (a b, FALSE))
(@ OR \a, b. (a TRUE, b))
(@ XOR \a, b. (a (b FALSE, TRUE), (b TRUE, FALSE)))

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
