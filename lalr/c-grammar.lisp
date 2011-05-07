
(define start translation_unit)

(define primary_expression
   (alt IDENTIFIER
        CONSTANT
        STRING_LITERAL
        (seq #\( expression #\))))

(define postfix_expression
   (alt primary_expression
        (seq postfix_expression #\[ expression #\])
        (seq postfix_expression #\( #\))
        (seq postfix_expression #\( argument_expression_list #\))
        (seq postfix_expression #\. IDENTIFIER)
        (seq postfix_expression PTR_OP IDENTIFIER)
        (seq postfix_expression INC_OP)
        (seq postfix_expression DEC_OP)))

(define argument_expression_list
   (alt assignment_expression
        (seq argument_expression_list #\, assignment_expression)))

(define unary_expression
   (alt postfix_expression
        (seq INC_OP unary_expression)
        (seq DEC_OP unary_expression)
        (seq unary_operator cast_expression)
        (seq SIZEOF unary_expression)
        (seq SIZEOF #\( type_name #\))))

(define unary_operator
   (alt #\& #\* #\+ #\- #\~ #\!))

(define cast_expression
   (alt unary_expression
        (seq #\( type_name #\) cast_expression)))

(define multiplicative_expression
   (alt cast_expression
        (seq multiplicative_expression #\* cast_expression)
        (seq multiplicative_expression #\/ cast_expression)
        (seq multiplicative_expression #\% cast_expression)))

(define additive_expression
   (alt multiplicative_expression
        (seq additive_expression #\+ multiplicative_expression)
        (seq additive_expression #\- multiplicative_expression)))

(define shift_expression
   (alt additive_expression
        (seq shift_expression LEFT_OP  additive_expression)
        (seq shift_expression RIGHT_OP additive_expression)))

(define relational_expression
   (alt shift_expression
        (seq relational_expression #\<   shift_expression)
        (seq relational_expression #\>   shift_expression)
        (seq relational_expression LE_OP shift_expression)
        (seq relational_expression GE_OP shift_expression)))

(define equality_expression
   (alt relational_expression
        (seq equality_expression EQ_OP relational_expression)
        (seq equality_expression NE_OP relational_expression)))

(define and_expression
   (alt equality_expression
        (seq and_expression #\& equality_expression)))

(define exclusive_or_expression
   (alt and_expression
        (seq exclusive_or_expression #\^ and_expression)))

(define inclusive_or_expression
   (alt exclusive_or_expression
        (seq inclusive_or_expression #\| exclusive_or_expression)))

(define logical_and_expression
   (alt inclusive_or_expression
        (seq logical_and_expression AND_OP inclusive_or_expression)))

(define logical_or_expression
   (alt logical_and_expression
        (seq logical_or_expression OR_OP logical_and_expression)))

(define conditional_expression
   (alt logical_or_expression
        (seq logical_or_expression #\? expression #\: conditional_expression)))

(define assignment_expression
   (alt conditional_expression
        (seq unary_expression assignment_operator assignment_expression)))

(define assignment_operator
   (alt #\=
        MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
        ADD_ASSIGN SUB_ASSIGN
        LEFT_ASSIGN RIGHT_ASSIGN
        AND_ASSIGN XOR_ASSIGN OR_ASSIGN))

(define expression
   (alt assignment_expression
        (seq expression #\, assignment_expression)))

(define constant_expression conditional_expression)

(define declaration
   (alt (seq declaration_specifiers #\;)
        (seq declaration_specifiers init_declarator_list #\;)))

(define declaration_specifiers
   (alt storage_class_specifier
        (seq storage_class_specifier declaration_specifiers)
        type_specifier
        (seq type_specifier declaration_specifiers)
        type_qualifier
        (seq type_qualifier declaration_specifiers)))

(define init_declarator_list
   (alt init_declarator
        (seq init_declarator_list #\, init_declarator)))

(define init_declarator
   (alt declarator
        (seq declarator #\= initializer)))

(define storage_class_specifier
   (alt TYPEDEF EXTERN STATIC AUTO REGISTER))

(define type_specifier
   (alt VOID CHAR SHORT INT LONG
        FLOAT DOUBLE SIGNED UNSIGNED
        struct_or_union_specifier
        enum_specifier TYPE_NAME))

(define struct_or_union_specifier
   (alt (seq struct_or_union IDENTIFIER #\{ struct_declaration_list #\})
        (seq struct_or_union #\{ struct_declaration_list #\})
        (seq struct_or_union IDENTIFIER)))

(define struct_or_union
   (alt STRUCT UNION))

(define struct_declaration_list
   (alt struct_declaration
        (seq struct_declaration_list struct_declaration)))

(define struct_declaration
   (seq specifier_qualifier_list struct_declarator_list #\;))

(define specifier_qualifier_list
   (alt (seq type_specifier specifier_qualifier_list)
        type_specifier
        (seq type_qualifier specifier_qualifier_list)
        type_qualifier))

(define struct_declarator_list
   (alt struct_declarator
        (seq struct_declarator_list #\, struct_declarator)))

(define struct_declarator
   (alt declarator
        (seq #\: constant_expression)
        (seq declarator #\: constant_expression)))

(define enum_specifier
   (alt (seq ENUM #\{ enumerator_list #\})
        (seq ENUM IDENTIFIER #\{ enumerator_list #\})
        (seq ENUM IDENTIFIER)))

(define enumerator_list
   (alt enumerator
        (seq enumerator_list #\, enumerator)))

(define enumerator
   (alt IDENTIFIER
        (seq IDENTIFIER #\= constant_expression)))

(define type_qualifier
   (alt CONST VOLATILE))

(define declarator
   (alt (seq pointer direct_declarator)
        direct_declarator))

(define direct_declarator
   (alt IDENTIFIER
        (seq #\( declarator #\))
        (seq direct_declarator #\[ constant_expression #\])
        (seq direct_declarator #\[ #\])
        (seq direct_declarator #\( parameter_type_list #\))
        (seq direct_declarator #\( identifier_list #\))
        (seq direct_declarator #\( #\))))

(define pointer
   (alt #\*
        (seq #\* type_qualifier_list)
        (seq #\* pointer)
        (seq #\* type_qualifier_list pointer)))

(define type_qualifier_list
   (alt type_qualifier
        (seq type_qualifier_list type_qualifier)))

(define parameter_type_list
   (alt parameter_list
        (seq parameter_list #\, ELLIPSIS)))

(define parameter_list
   (alt parameter_declaration
        (seq parameter_list #\, parameter_declaration)))

(define parameter_declaration
   (alt (seq declaration_specifiers declarator)
        (seq declaration_specifiers abstract_declarator)
        declaration_specifiers))

(define identifier_list
   (alt IDENTIFIER
        (seq identifier_list #\, IDENTIFIER)))

(define type_name
   (alt specifier_qualifier_list
        (seq specifier_qualifier_list abstract_declarator)))

(define abstract_declarator
   (alt pointer
        direct_abstract_declarator
        (seq pointer direct_abstract_declarator)))

(define direct_abstract_declarator
   (alt (seq #\( abstract_declarator #\))
        (seq #\[ #\])
        (seq #\[ constant_expression #\])
        (seq direct_abstract_declarator #\[ #\])
        (seq direct_abstract_declarator #\[ constant_expression #\])
        (seq #\( #\))
        (seq #\( parameter_type_list #\))
        (seq direct_abstract_declarator #\( #\))
        (seq direct_abstract_declarator #\( parameter_type_list #\))))

(define initializer
   (alt assignment_expression
        (seq #\{ initializer_list #\})
        (seq #\{ initializer_list #\, #\})))

(define initializer_list
   (alt initializer
        (seq initializer_list #\, initializer)))

(define statement
   (alt labeled_statement
        compound_statement
        expression_statement
        selection_statement
        iteration_statement
        jump_statement))

(define labeled_statement
   (alt (seq IDENTIFIER #\: statement)
        (seq CASE constant_expression #\: statement)
        (seq DEFAULT #\: statement)))

(define compound_statement
   (alt (seq #\{ #\})
        (seq #\{ statement_list #\})
        (seq #\{ declaration_list #\})
        (seq #\{ declaration_list statement_list #\})))

(define declaration_list
   (alt declaration
        (seq declaration_list declaration)))

(define statement_list
   (alt statement (seq statement_list statement)))

(define expression_statement
   (alt #\; (seq expression #\;)))

(define selection_statement
   (alt (seq IF #\( expression #\) statement)
        (seq IF #\( expression #\) statement ELSE statement)
        (seq SWITCH #\( expression #\) statement)))

(define iteration_statement
   (alt (seq WHILE #\( expression #\) statement)
        (seq DO statement WHILE #\( expression #\) #\;)
        (seq FOR #\( expression_statement expression_statement #\) statement)
        (seq FOR #\( expression_statement expression_statement expression #\) statement)))

(define jump_statement
   (alt (seq GOTO IDENTIFIER #\;)
        (seq CONTINUE #\;)
        (seq BREAK #\;)
        (seq RETURN #\;)
        (seq RETURN expression #\;)))

(define translation_unit
   (alt external_declaration
        (seq translation_unit external_declaration)))

(define external_declaration
   (alt function_definition declaration))

(define function_definition
   (alt (seq declaration_specifiers declarator declaration_list compound_statement)
        (seq declaration_specifiers declarator compound_statement)
        (seq declarator declaration_list compound_statement)
        (seq declarator compound_statement)))

