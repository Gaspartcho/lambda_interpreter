(* "std.lp")

%($& (THREE "abo"  dwdd   , "caf"))%


%([#$?] (SUCC (SUCC THREE)))%

(@[$] OTHER_OR \a, b. (NOT (AND (NOT a), (NOT b))))
(@[$] OTHER_XOR \a, b. (AND (OR a, b), (NOT (AND a, b))))

%
([+][#$] (OR, TRUE, TRUE))
([#$][-]# (XOR, TRUE, FALSE))
%
