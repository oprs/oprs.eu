
; lexemes - can reference each other, no event thrown

(lexeme DIGIT    (alt #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9))

(lexeme ALPHA    (alt #\A #\B #\C #\D #\E #\F #\G #\H #\I #\J
                      #\K #\L #\M #\N #\O #\P #\Q #\R #\S #\T
                      #\U #\V #\W #\X #\Y #\Z
                      #\a #\b #\c #\d #\e #\f #\g #\h #\i #\j
                      #\k #\l #\m #\n #\o #\p #\q #\r #\s #\t
                      #\u #\v #\w #\x #\y #\z #\_))

(lexeme HEXA     (alt #\A #\B #\C #\D #\E #\F
                      #\a #\b #\c #\d #\e #\f
                      DIGIT))

(lexeme EX       (seq (alt #\E #\e)
                      (opt (alt #\+ #\-))
                      (seq DIGIT (rep DIGIT))))

(lexeme IS       (rep (alt #\U #\u #\L #\l)))

(lexeme FS            (alt #\F #\f #\L #\l))


; tokens - can only reference lexemes, throw events

(token IDENTIFIER    (seq ALPHA (rep (alt ALPHA DIGIT))))

(token AUTO          "auto"    )
(token BREAK         "break"   )
(token CASE          "case"    )
(token CHAR          "char"    )
(token CONST         "const"   )
(token CONTINUE      "continue")
(token DEFAULT       "default" )
(token DO            "do"      )
(token DOUBLE        "double"  )
(token ELSE          "else"    )
(token ENUM          "enum"    )
(token EXTERN        "extern"  )
(token FLOAT         "float"   )
(token FOR           "for"     )
(token GOTO          "goto"    )
(token IF            "if"      )
(token INT           "int"     )
(token LONG          "long"    )
(token REGISTER      "register")
(token RETURN        "return"  )
(token SHORT         "short"   )
(token SIGNED        "signed"  )
(token SIZEOF        "sizeof"  )
(token STATIC        "static"  )
(token STRUCT        "struct"  )
(token SWITCH        "switch"  )
(token TYPEDEF       "typedef" )
(token UNION         "union"   )
(token UNSIGNED      "unsigned")
(token VOID          "void"    )
(token VOLATILE      "volatile")
(token WHILE         "while"   )
(token ELLIPSIS      "..."     )
(token RIGHT_ASSIGN  ">>="     )
(token LEFT_ASSIGN   "<<="     )
(token ADD_ASSIGN    "+="      )
(token SUB_ASSIGN    "-="      )
(token MUL_ASSIGN    "*="      )
(token DIV_ASSIGN    "/="      )
(token MOD_ASSIGN    "%="      )
(token AND_ASSIGN    "&="      )
(token XOR_ASSIGN    "^="      )
(token OR_ASSIGN     "|="      )
(token RIGHT_OP      ">>"      )
(token LEFT_OP       "<<"      )
(token INC_OP        "++"      )
(token DEC_OP        "--"      )
(token PTR_OP        "->"      )
(token AND_OP        "&&"      )
(token OR_OP         "||"      )
(token LE_OP         "<="      )
(token GE_OP         ">="      )
(token EQ_OP         "=="      )
(token NE_OP         "!="      )

