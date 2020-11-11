lexer grammar PherriteLexer;

// Symbols
LPAREN    : '(' ;
RPAREN    : ')' ;
LBRACE    : '{' ;
RBRACE    : '}' ;
COLON     : ':' ;
SEMICOLON : ';' ;
STAR      : '*' ;
SLASH     : '/' ;
PLUS      : '+' ;
MINUS     : '-' ;

// Keywords
FN     : 'fn' ;
MACRO  : 'macro' ;
LET    : 'let' ;
VAR    : 'var' ;
RETURN : 'return' ;
DEBUG  : 'debug' ;

// Number literals
ZERO    : '0' ;
DEC_LIT : [1-9][0-9]* ;
OCT_LIT : '0'[0-7]+ ;
HEX_LIT : '0x'[0-9a-fA-F]+ ;
BIN_LIT : '0b'[01]+ ;

// String literals
SINGLE_STR : '"' ('\\"' | ~('\n' | '\r'))*? '"' ;
MULTI_STR  : '"""' ('\\"' | ~('\n' | '\r'))*? '"""' ;

ID : [A-Za-z_][A-Za-z0-9_]* ;

WS : [\t\n\r ]+ -> skip;
