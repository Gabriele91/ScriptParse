#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "Tokenizer.h"
#include "TreeNode.h"
#include "ErrorParse.h"

/**

    @autor Gabriele Di Bari
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 00:24
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 21:48
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-23 00:45
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-23 11:35
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-23 16:15
    @date  ( yyyy-mm-dd, hh:mm ) 2012-12-23 19:58
    @version 0.4.9.5

    //recursive descent parser (exp) (WORK!!!!)

    *  <statement> := <statement_if> | 
                      <statement_while> | 
                      <statement_do> | 
                      <statement_assignament> |
                      <statement_function> |
                      <statement_call> |
					  <statement_break>|
					  <statement_continue>|
					  <statement_global>|
					  <statement_local>
                      

    *  <statement_if> := if '(' <exp> ')' '{' [{<statement>}]  '}' { <statement_elif> } [ <statement_else> ]
    *  <statement_elif> := elif '(' <exp> ')' '{' [{<statement>}]  '}'
    *  <statement_else> := else '{' [{<statement>}]  '}'
    *  <statement_while> := while '(' <exp> ')' '{' [{<statement>}]  '}'
    *  <statement_for> := for '(' [{<statement_assignament>}] , <exp>, [{<statement_assignament>}] ')' '{' [{<statement>}]  '}'
    *  <statement_do> := do '{' [{<statement>}] '}'  while '(' <exp> ')'
    *  <statement_assignament> := <VARIABLE> '=' <exp>
    *  <statement_function> := 'def' <VARIABLE> ['(' [<ARGS>] ')']'{' [{<statement>}] '}'
    *  <statement_return>    := 'return' <exp>
    *  <statement_break>    := 'break'
    *  <statement_continue>    := 'continue'
    *  <statement_global>    := 'global' <VARIABLE>{','<VARIABLE>}
    *  <statement_local>    := 'local' <VARIABLE>{','<VARIABLE>}
    *  <call> :=  <VARIABLE> '(' <exp>  { ',' <exp> } ')'
    *  <ARGS> :=  <VARIABLE> { ',' <VARIABLE> }
    *
    ************************************************
    *** EXPRESCION PARSE ***************************
    ************************************************
    *   exp :=  <compare> { <op_logic> <compare> }    
    *    op_logic: '&&' | '||'

    *   compare := <base> { <op_compare> <base> }
    *   op_compare := '=='| '!=' | '<' | '>' | '<=' | '>=' 

    *   base  := <term> { <op_livel1> <term> }
    *    op_livel1 : '+' | '-'

    *   term := <factor> { <op_livel2> <factor> }
    *    op_livel2 : '*' | '/'

    *   factor := [<unitary_op>] <NUMBER> |
                  [<unitary_op>] <VARIABLE> |
                  <STRING> | 
                  <statement_call> |
                  <statement_nameless_function>[<nemeless_call>] |
                  '(' <exp> ')'
        
    *   <statement_nameless_function>:= 'def' ['(' [<ARGS>] ')']'{' [{<statement>}] '}'
    *   <nemeless_call>:='(' <exp>  { ',' <exp> } ')'
    *   unitary_op := '-' | '!'

    struct SyntaxTree{
        [...]
        Tokenizer tkobj;
        [...]    
        bool ParserFactor();
        bool ParseTerm()
        bool ParseBase()
        bool ParseExp()

        bool StartParse(const std::string & script)
        
        [...]
    }
    *** EXAPLE *********************************************************************************
    SyntaxTree testExp;
    std::cout <<( rep.StartParse(" (-9)+2*((2*3-5)*5)/10 ") ? "valid" : "invalid" )<< std::endl;
    ********************************************************************************************
    
    //MAKE TREE 
    // exemple:
    //
    //   (1+2)*5
    //
    //       *
    //      / \
    //     +   5
    //    / \
    //   1   2
    //
    //   1+2*5
    //
    //     +
    //    / \
    //   1   *
    //      / \
    //     2   5
    //

    //into exp setp1: exp--->
    //                       |
    //                         v
    //    into base, setp1 : base->term->factor      '1' !number! Push Tree leaf  
    //    into base, step2 : base<-term<-factor      '+' !op!     Push Tree node 
    //    into base, step3 : base---------->
    //     |                               |
    //     |                               |
    //     |                               v
    //     v          into term, step1 : term->factor            '2' !number! Push Tree leaf
    //     child--->  into term, step2 : term<-factor            '*' !op!     Push Tree node 
    //                into term, step3 : term->factor            '5' !number! Push Tree leaf
    
    ***************
    FUNCTION TREE:
    ***************
    *
    *                   < def >
    *                /           \
    *               /             \
    *      none(header|name)       \ 
    *         /|\              <staments>
    *        / | \
    *       < args >
    

    **********
    CALL TREE:
    **********
    *      <variable>
    *        /|\       
    *       / | \
    *       <exp>    
    *

    ************************
    LESS NAME FUNCTION TREE:
    ************************
    *
    *                   < def >
    *                /           \
    *               /             \
    *   none(lessname_header)      \ 
    *         /|\              <staments>
    *        / | \
    *       < args >

    ********************
    LESS NAME CALL TREE:
    ********************
    *        <NONE>
    *        /  \\\      
    *       /    \\\
    *    <def>   <exp>
    *
    
    *****************
    ASSIGNAMENT TREE:
    *****************
    *         <=>
    *        /   \       
    *       /     \
    *<variable>   <exp>
    *
    
    *****************
    IF TREE:
    *****************
    *         <if>______________________
    *        /   \           \\\\\      \
    *       /     \           \\\\\      \
    *    <exp>   <statement>   <elif>    <else>
    *    

    *****************
    ELIF TREE:
    *****************
    *        <elif>
    *        /   \
    *       /     \
    *    <exp>   <statement>
    *    
    
    *****************
    ELSE TREE:
    *****************
    *     <else>
    *       |
    *       |
    *   <statement>
    *

    *****************
    WHILE TREE:
    *****************
    *       <while>
    *        /   \       
    *       /     \
    *    <exp>   <statement>
    *  
    *****************
    DO-WHILE TREE:
    *****************
    *         <do>
    *        /   \       
    *       /     \
    * <statement> <exp>
    *	  

    *****************
    FOR TREE:
    *****************
    *        <for>_____________
    *        /				   \       
    *       /					\
    *   none(for_header)	<statement>
	*    |     |      |
    *    |     |      |
	*    |    <exp>   |
	*   none         none
	*  | | |        | | |
	* statement_assignament

	*****************
    GLOBAL TREE:
    *****************
    *    <global>
    *       |
    *       |
    *   <variable>
    *	
	*****************
    LOCAL TREE:
    *****************
    *    <local>
    *       |
    *       |
    *   <variable>
    *
*/

struct SyntaxTree{
	
    

    std::string script;
    Tokenizer tkn;
    ErrorParse* errors;

    
    TreeNode* ParserFactor();
    TreeNode* ParseTerm();
    TreeNode* ParseBase();
    TreeNode* ParseCompare();
    TreeNode* ParseExp();

    TreeNode* ParseIf();
    TreeNode* ParseElif();
    TreeNode* ParseElse();

    TreeNode* ParseWhile();
    TreeNode* ParseDo();
    TreeNode* ParseFor();
	TreeNode* ParseBreak();
	TreeNode* ParseContinue();

	TreeNode* ParseLocal();
	TreeNode* ParseGlobal();

    TreeNode* ParseAssignament();
    
    TreeNode* ParseCall();
    TreeNode* ParseLessNameCall(TreeNode *lessNameFunction);
    
    /***
    *                   < def >
    *                /           \
    *               /             \
    *      none(header|name)       \ 
    *         /|\              <staments>
    *        / | \
    *       < args >
    */
    TreeNode* ParseDef();
    TreeNode* ParseLessNameDef();
    TreeNode* ParseReturn();
	

    TreeNode* ParseStatement();
    TreeNode* ParseStatements();

    TreeNode* StartParse(const std::string & script,
						 ErrorParse* errors);

};


#endif