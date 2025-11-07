


%
	\lprogSTD: Boolean

	REQUIRES: nothing.

	Basic operations for boolean arithmetic.
%


(@ TRUE \x y. x)
(@ FALSE \x y. y)

(@ NOT \b. (b FALSE TRUE))
(@ AND \a, b. (a b, a))
(@ OR  \a, b. (a a, b))
(@[$] IMPL \a, b. (a b, (NOT a)))
(@[$] XOR  \a, b. (a (NOT b), b))
(@[$] IFF  \a, b. (a b, (NOT b)))

(@ IF_THEN_ELSE \p, a, b. (p a, b))
