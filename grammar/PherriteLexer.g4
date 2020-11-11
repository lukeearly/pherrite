lexer grammar PherriteLexer;

// Symbols
LPAREN : '(' ;
RPAREN : ')' ;
LBRACE : '{' ;
RBRACE : '}' ;
SEMICOLON : ';' ;

// Keywords
FN : 'fn' ;
//MACRO : 'macro' ;
LET : 'let' ;
VAR : 'var' ;
RETURN : 'return' ;
DEBUG : 'debug' ;

// Literals
ZERO : '0' ;
DEC_LIT : [1-9][0-9]* ;
OCT_LIT : '0'[0-9]+ ;
HEX_LIT : '0x'[0-9a-fA-F]+ ;
BIN_LIT : '0b'[01]+ ;
ID : [A-Za-z_][A-Za-z0-9_]* ;

WS : [\t\n\r ]+ -> skip;
