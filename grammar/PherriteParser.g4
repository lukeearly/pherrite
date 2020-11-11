parser grammar PherriteParser;

options { tokenVocab = PherriteLexer; }

topLevel : definition* ;

definition : fundef | macrodef /* | typedef | import */;

macrodef : MACRO name=ID params=paramList body=block ;

fundef : FN name=ID params=paramList body=block ;

paramList : LPAREN RPAREN ;

block : LBRACE (statement SEMICOLON)* RBRACE ;

statement : block | returnStatement | debugStatement;

returnStatement : RETURN | RETURN expr ;

debugStatement : DEBUG | DEBUG expr ;

expr: left=expr op=(STAR|SLASH) right=expr #binaryExpr
    | left=expr op=(PLUS|MINUS) right=expr #binaryExpr
    | left=expr right=expr #binaryExpr
    | op=(PLUS|MINUS|STAR) expr #unaryExpr
    | LPAREN expr RPAREN #parenExpr
    | (ZERO | DEC_LIT | OCT_LIT | HEX_LIT | BIN_LIT) #numLitExpr
    | (SINGLE_STR | MULTI_STR) #strLitExpr
    | ID #idExpr
    ;

