
(define digit     (alt #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9))

(define alpha     (alt #\A #\B #\C #\D #\E #\F #\G #\H #\I #\J
                       #\K #\L #\M #\N #\O #\P #\Q #\R #\S #\T
                       #\U #\V #\W #\X #\Y #\Z
                       #\a #\b #\c #\d #\e #\f #\g #\h #\i #\j
                       #\k #\l #\m #\n #\o #\p #\q #\r #\s #\t
                       #\u #\v #\w #\x #\y #\z #\_))

(define hexa      (alt #\A #\B #\C #\D #\E #\F 
                       #\a #\b #\c #\d #\e #\f
                       digit))

(define ex        (seq (alt #\E #\e)
                       (opt (alt #\+ #\-))
                       (seq digit (rep digit))))

(define is        (rep (alt #\U #\u #\L #\l)))

(define fs             (alt #\F #\f #\L #\l))

(define identifier (seq alpha
                        (rep (alt alpha digit))))

(define auto      "auto"    )
(define break     "break"   )
(define case      "case"    )
(define char      "char"    )
(define const     "const"   )
(define continue  "continue")
(define default   "default" )
(define do        "do"      )
(define double    "double"  )
(define else      "else"    )
(define enum      "enum"    )
(define extern    "extern"  )
(define float     "float"   )
(define for       "for"     )
(define goto      "goto"    )
(define if        "if"      )
(define int       "int"     )
(define long      "long"    )
(define register  "register")
(define return    "return"  )
(define short     "short"   )
(define signed    "signed"  )
(define sizeof    "sizeof"  )
(define static    "static"  )
(define struct    "struct"  )
(define switch    "switch"  )
(define typedef   "typedef" )
(define union     "union"   )
(define unsigned  "unsigned")
(define void      "void"    )
(define volatile  "volatile")
(define while     "while"   )

(define ellipsis     "..."  )
(define right_assign ">>="  )
(define left_assign  "<<="  )
(define add_assign   "+="   )
(define sub_assign   "-="   )
(define mul_assign   "*="   )
(define div_assign   "/="   )
(define mod_assign   "%="   )
(define and_assign   "&="   )
(define xor_assign   "^="   )
(define or_assign    "|="   )
(define right_op     ">>"   )
(define left_op      "<<"   )
(define inc_op       "++"   )
(define dec_op       "--"   )
(define ptr_op       "->"   )
(define and_op       "&&"   )
(define or_op        "||"   )
(define le_op        "<="   )
(define ge_op        ">="   )
(define eq_op        "=="   )
(define ne_op        "!="   )

