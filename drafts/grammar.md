
```
/* TOP LEVEL */

// TODO

/* STATEMENTS */

statement_list:
	statement {statement}
statement:
	statement_if
	statement_select
	statement_where
	statement_switch
	statement_forever
	statement_while
	statement_repeat
	statement_continue
	statement_exit
	statement_break
	statement_return
	statement_pass
	statement_incdec
	statement_assign
statement_if:
	"if" expr "then" statement_list ["else" statement_list] end
statement_select:
	"select" select_case_list ["default" ":" statement_list] end
statement_where:
	"where" expr ":" statement
statement_switch:
	"switch" expr switch_case_list ["default" ":" statement_list] end
statement_forever:
	"forever" loop_body
statement_while:
	"while" expr loop_body
statement_repeat:
	"repeat" statement_list "until" expr ";"
statement_continue:
	"continue" [ident] ";"
statement_exit:
	"exit" [ident] ";"
statement_break:
	"break" [ident] ";"
statement_return:
	"return" [expr] ";"
statement_pass:
	"pass" ";"
statement_incdec:
	"inc" expr ";"
	"dec" expr ";"
statement_assign:
	expr name_decl {"," name_decl} ";"
	expr "=" expr ";"
	expr ";"
name_decl:
	["@"] ident [type_suffix] ["=" expr]
select_case_list:
	select_case {select_case}
switch_case_list:
	switch_case {switch_case}
select_case:
	"case" expr ":" statement_list
switch_case:
	"case" switch_case_match ":" statement_list
switch_case_match:
	expr {"," expr}
loop_body:
	"loop" statement_list end
	"do" statement_list end
end:
	"end" [":" ident] [";"]

/* EXPRESSIONS */

expr:
	expr2 {bin_op expr2} /* The priority of two-operand operations
	                        is not covered by this grammar for simplicity. */

bin_op:
	"|"
	"&"
	"<"
	"<="
	"=="
	"!="
	">"
	">="
	"+"
	"-"
	"*"
	"/"
	"%"

expr2:
	"@" expr2
	"^" expr2
	"[" "]" expr2
	"[" expr "]" expr2
	"function" "(" arg_list ")" "->" expr2
	"-" expr3
	"!" expr3
	expr3

expr3:
	expr4 "[" expr "]"
	expr4 "(" [param_list] ")"

expr4:
	primary { deref "." ident } deref

deref:
	{"^"}

primary:
	"(" expr ")"
	"false"
	"true"
	"null"
	"NULL"
	ident
	literal

param_list:
	expr {"," expr}

type_suffix:
	"^" [type_suffix]
	"@" [type_suffix]
	"[" [expr] "]" [type_suffix]
	"(" arg_list ")" [type_suffix]

arg_list:
	arg_decl_list { ";" arg_decl_list }
arg_decl_list:
	expr arg_decl {"," arg_decl}
arg_decl:
	["@"] ident [type_suffix] ["=" expr]
```

