parser grammar PherriteParser;

options { tokenVocab = PherriteLexer; }

topLevel : definition* ;

definition : fundef ;

//macrodef : MACRO ID paramList block ;

fundef : FN ID paramList block ;

paramList : LPAREN RPAREN ;

block : LBRACE (statement SEMICOLON)* RBRACE ;

statement : block | returnStatement | debugStatement;

returnStatement : RETURN | RETURN expr ;

debugStatement : DEBUG | DEBUG expr ;

expr: ID | numberLiteral ;

numberLiteral : ZERO | DEC_LIT | OCT_LIT | HEX_LIT | BIN_LIT ;
