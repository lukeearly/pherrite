parser grammar PherriteParser;

options { tokenVocab = PherriteLexer; }

topLevel : definition* ;

definition : funDef | funDecl /* | typedef | import */;

//macrodef : MACRO name=ID params=paramList body=block ;

funProto : FN name=ID params=paramList (COLON retType=typeExpr)? ;

funDef : proto=funProto body=block ;

funDecl : proto=funProto SEMICOLON ;

typeExpr : builtinType /* | ID */;

paramList : LPAREN RPAREN ;
//paramList : LPAREN (param (COMMA param))? RPAREN ;
//
//param : ID COLON ID ;

block : LBRACE (statement SEMICOLON)* RBRACE ;

statement : RETURN expr #returnValue
          | DEBUG expr #debugValue
          | LET name=ID (COLON type=typeExpr)? ASSIGN value=expr #letState;

//expr: left=expr op=(STAR|SLASH) right=expr #binaryExpr
    //| left=expr op=(PLUS|MINUS) right=expr #binaryExpr
    //| left=expr right=expr #binaryExpr
    //| op=(PLUS|MINUS|STAR) expr #unaryExpr
    //| LPAREN expr RPAREN #parenExpr
    //| (ZERO | DEC_LIT | OCT_LIT | HEX_LIT | BIN_LIT) #numLitExpr
    //| (SINGLE_STR | MULTI_STR) #strLitExpr
    //| ID #idExpr
    //;

expr: (ZERO | DEC_LIT | OCT_LIT | HEX_LIT | BIN_LIT) #numLitExpr
    | callee=ID LPAREN RPAREN #callExpr
    | ID #idExpr ;

builtinType : INT | INTN | REAL | REALN ;
