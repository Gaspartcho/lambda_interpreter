


%
	\lprogSTD: Definitions

	REQUIRES: nothing.

	Most basic definitions to be used as building block for more complex features.
%


(@ I \x. x)
(@ SELF \x. (x, x))

(@ REC SELF)
($! (@ DEF_DIS \name, func. ($! (@ name, func))))
(DEF_DIS DEF_REC \name, func. (DEF_DIS name (SELF, func)))
