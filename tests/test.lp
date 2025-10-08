(* "std.lp")

%($& (THREE "abo"  dwdd   , "caf"))%


%([#$?] (SUCC (SUCC THREE)))%

(@[$] OTHER_OR \a, b. (NOT (AND (NOT a), (NOT b))))
(@[$] OTHER_XOR \a, b. (AND (OR a, b), (NOT (AND a, b))))

([+][#$] (OTHER_XOR, TRUE, FALSE))
([+][#$][-]& (XOR, TRUE, FALSE))
