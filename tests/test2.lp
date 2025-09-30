
(@ I \x. x)

(@ TRUE \x y. x)
(@ FALSE \x y. y)

(@ AND \x y. (x y FALSE))
(@ OR \x y. (x TRUE y))
(@ NOT \x. (x FALSE TRUE))


(@ SUCC \n f x. (f, (n f x)))
(@ ONE \f x. (f x))
(@$ TWO (SUCC ONE))
(@$ THREE (SUCC TWO))
