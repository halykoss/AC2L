//TODO: Aggiungere la stampa del return e dei loop

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void LISP_operando_node(operando_node* node);
void LISP_pre_post_inc_node(pre_post_inc_node* node);

char* scope = NULL;

void LISP_termine_node(termine_node* node){
  if(node->unary_sign == '-')  {
      printf(" ( -" );
      LISP_operando_node(node->op);
      printf(" )" );
      return;
    }

  LISP_operando_node(node->op);
}

void LISP_exp_node(exp_node* node);

void LISP_array_node(array_node* node);

void LISP_base_list_exp(list_exp_node* ls);

void LISP_print_var_node(var_node* node){
  switch (node->type) {
    case VAR_INT:
      printf(" %s", node->id );
    break;

    case VAR_ARRAY:
    printf(" ( aref ");
    printf(" %s", node->id );
    LISP_array_node(node->array);
    printf(" )");
    break;
  }
}
void LISP_operando_node(operando_node* node){
  switch (node->type) {

    case TERM_NUMBER:
      printf(" %d", node->num );
    break;

    case TERM_VARIABLE:
    LISP_print_var_node(node->var);
    break;

    case TERM_FUNCALL:
      printf(" (");
      printf(" %s",node->func->id );
      LISP_base_list_exp(node->func->ls_exp);
      printf("%s", " )");
    break;

    case TERM_EXP:
      LISP_exp_node(node->exp);
    break;

    case TERM_PP:
      printf(" (");
      LISP_pre_post_inc_node(node->pp);
      printf(" )");
    break;
  }
}

void LISP_switch_op(EXP_Type val){
  switch (val) {
    case OP_PLUS:
    printf(" +");
    break;

    case OP_MINUS:
    printf(" -");
    break;

    case OP_MULT:
    printf(" *");
    break;

    case OP_DIV:
    printf(" /");
    break;

    case OP_MOD:
    printf(" %s", "%" );
    break;

    case OP_GREATER:
    printf(" >");
    break;

    case OP_LESS:
    printf(" <");
    break;

    case OP_AND:
    printf(" logand");
    break;

    case OP_OR:
    printf(" logor");
    break;

    case OP_ANDL:
    printf(" and");
    break;

    case OP_ORL:
    printf(" or");
    break;

    case OP_GREATER_EQ:
    printf("%s", " >=");
    break;

    case OP_LESS_EQ:
    printf("%s", " <=");
    break;

    case OP_NOTEQ:
    printf("%s", " /=");
    break;

    case OP_EQ:
    printf("%s", " =");
    break;
  }
}

void LISP_exp_node(exp_node* node){
  switch (node->type) {

    case OP_NUMERIC:
      LISP_termine_node(node->term);
    break;

    case OP_TERNARY:
      printf(" (if ");
      LISP_exp_node(node->term1);
      LISP_exp_node(node->term2);
      LISP_exp_node(node->term3);
      printf(" )" );
    break;

    case OP_NOT:
      printf(" ( not");
      LISP_exp_node(node->term1);
      printf(" )");
    break;

    default:
      printf(" (" );
      LISP_switch_op(node->type);
      LISP_exp_node(node->term1);
      LISP_exp_node(node->term2);
      printf(" )" );
    break;
  }
}

void LISP_args_node(args_node* args){
  if(args == NULL)
    return ;

  printf(" %s", args->var->id );
  LISP_args_node(args->next);
}

void LISP_block_node(block_node* node);

void LISP_func_node(func_node* node){
  scope = strdup(node->id);
  printf("( defun %s", node->id );
  printf(" (" );
  LISP_args_node(node->args);
  printf(" )");
  printf("%s", " (" );
  LISP_block_node(node->block);
  printf("%s", " )" );
  printf(" )" );
}

void LISP_array_node(array_node* node){
  if (node == NULL)
    return ;

  LISP_exp_node(node->dim);
  LISP_array_node(node->next);
}

void LISP_base_list_exp(list_exp_node* ls){
  if(ls == NULL)
    return ;

  LISP_exp_node(ls->exp);
  LISP_base_list_exp(ls->next);

}

void LISP_as_ar_list_node(as_ar_list_node* ls);

void LISP_array_content_node(array_content_node* node){
  printf("%s", "(");
  if(node->ls != NULL){
    LISP_base_list_exp(node->ls);
  }
  else{
    LISP_as_ar_list_node(node->content);
  }
  printf("%s", " )" );
}

void LISP_as_ar_list_node(as_ar_list_node* ls){
  if(ls == NULL)
    return;

  LISP_array_content_node(ls->node);
  printf("%s", " ");
  LISP_as_ar_list_node(ls->next);
}

void LISP_print_def_var_node_val(def_var_node* node){
  switch (node->var->type) {
    case VAR_INT:
      if(node->assign == NULL)
        printf(" 0" );
      else
        LISP_exp_node(node->assign->exp);
    break;

    case VAR_ARRAY:
    printf("( make-array '(");
    LISP_array_node(node->var->array);
    printf(" ) ");
    if(node->assign != NULL)
    {
      printf("%s", ":initial-contents '");
      LISP_array_content_node(node->assign->array);
    }
    printf("%s", " )");
    break;
  }
}
void LISP_def_var_node(def_var_node* node){
  printf(" (defvar %s",node->var->id);
  LISP_print_def_var_node_val(node);
  printf(")");
}

void LISP_let_def_var_node(def_var_node* node){
  printf(" ( %s",node->var->id);
  LISP_print_def_var_node_val(node);
  printf(")");
}

void LISP_def_node(def_node* def){
  if(def == NULL)
    return;

  switch (def->type) {
    case DEF_FUNC:
      LISP_func_node(def->func);
      break;
    case DEF_VAR:
      LISP_def_var_node(def->def_var);
      break;
  }

  LISP_def_node(def->next);
}

void LISP_lisp_code_node(lisp_code_node* node){
  if(node == NULL)
    return;

  printf("%s", node->value);
  LISP_lisp_code_node(node->next);
}

void LISP_else_node(else_node* else_n){
  if(else_n == NULL)
    return ;

  printf("%s"," ( " );
  LISP_block_node(else_n->block);
  printf("%s"," ) " );
}

void LISP_if_node(if_node* node){
  if(node->else_n == NULL){
    printf("%s"," when" );
    LISP_exp_node(node->exp);
    printf("%s"," ( " );
    LISP_block_node(node->block);
    printf("%s"," ) " );
  }
  else{
    printf("%s"," if" );
    LISP_exp_node(node->exp);
    printf("%s"," ( " );
    LISP_block_node(node->block);
    printf("%s"," ) " );
    LISP_else_node(node->else_n);
  }
}

void LISP_print_set_var(var_node* node) {
  switch (node->type) {
    case VAR_INT:
      printf("%s", " setq" );
    break;

    case VAR_ARRAY:
    printf("%s", " setf" );
    break;
  }
}

void LISP_pre_post_inc_node(pre_post_inc_node* node){
  switch (node->p_type) {
    case POST:
      printf(" prog1" );
    break;

    case PRE:
      printf(" prog2" );
    break;
  }
  LISP_print_var_node(node->var);
  printf(" (" );
  LISP_print_set_var(node->var);
  LISP_print_var_node(node->var);
  printf("%s %c", " (",node->sign);
  LISP_print_var_node(node->var);
  printf("%s", " 1 )");
  printf(" )" );
}

void LISP_var_list_node(var_list_node* ls){
  if(ls == NULL)
    return;

  printf("%s","  progn " );
  printf("%s", " (" );
  LISP_print_set_var(ls->var);
  LISP_print_var_node(ls->var);
  printf("%s"," ( read )");
  printf("%s", " )" );
  if(ls->next == NULL)
    return ;
  printf("%s", " (" );
  LISP_var_list_node(ls->next);
  printf("%s", " )" );
}

char* LISP_normalize_string(char* string){
  int len = strlen(string);
  char* new = ALLOC(char,len);

  for(int i=0; i < len; i++){
    if(string[i] == '%'){
      if( i + 1 < len && string[i+1] == 'd' ){
          new[i++] = '~';
          new[i] = 'D';
        }
      }
    else if(string[i] == '\\'){
      if( i + 1 < len && string[i+1] == 'n' ){
          new[i++] = '~';
          new[i] = '%';
        }
      }
      else
        new[i] = string[i];
    }
  return new;
}
void LISP_output_node(output_node* node){
  printf("%s \"%s\"", " format t" , LISP_normalize_string(node->string) );
  LISP_base_list_exp(node->exp_ls);
}

void LISP_loop_node(loop_node* node){
  switch (node->type) {
    case LOOP_WHILE:
      printf("%s", " loop while" );
      LISP_exp_node(node->exp);
      printf("%s"," do " );
      printf(" %s", "(" );
      LISP_block_node(node->block);
      printf(" %s", ")" );
    break;

    case LOOP_DO_WHILE:
      printf(" %s", " loop do" );
      printf(" %s", "(" );
      LISP_block_node(node->block);
      printf(" %s", ")" );
      printf(" %s", "while" );
      LISP_exp_node(node->exp);
    break;

  }
}

void LISP_block_operation(block_node* node){
  switch (node->type) {
    case BLOCK_ASSIGN:
    LISP_print_set_var(((def_var_node*)node->node)->var);
    LISP_print_var_node(((def_var_node*)node->node)->var);
    LISP_exp_node(((def_var_node*)node->node)->assign->exp);
    break;

    case BLOCK_IF:
      LISP_if_node((if_node*) node->node);
    break;

    case BLOCK_PP:
      LISP_pre_post_inc_node((pre_post_inc_node*) node->node);
    break;

    case BLOCK_FUNCALL:
      printf(" %s",((funcall_node*) node->node)->id );
      LISP_base_list_exp(((funcall_node*) node->node)->ls_exp);
    break;

    case BLOCK_RETURN:
      printf("return-from %s", scope );
      LISP_exp_node(((return_node*) node->node)->exp);
    break;

    case BLOCK_INPUT:
      LISP_var_list_node(((input_node*) node->node)->ls);
    break;

    case BLOCK_OUTPUT:
      LISP_output_node((output_node*) node->node);
    break;

    case BLOCK_LOOP:
      LISP_loop_node((loop_node*) node->node);
    break;
  }
}

void LISP_block_node(block_node* node){
  if(node != NULL)
    if(node->type == BLOCK_DEF){
      printf("%s","  let ");
      printf("%s", " (" );
      LISP_let_def_var_node((def_var_node*) node->node);
      printf("%s", " )" );
      printf("%s", " (" );
      LISP_block_node(node->next);
      printf("%s"," )" );
    } else if(node->next == NULL){
      LISP_block_operation(node);
    }else{
          printf("%s","  progn " );
          printf("%s", " (" );
          LISP_block_operation(node);
          printf("%s", " )" );
          printf("%s", " (" );
          LISP_block_node(node->next);
          printf("%s", " )" );
        }
}

void LISP_AST_node(AST_node*  node){
  LISP_def_node(node->def);
  LISP_lisp_code_node(node->lisp);
}
