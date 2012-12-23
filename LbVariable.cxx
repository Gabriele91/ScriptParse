
#include "LbVariable.h"

///////////////////
//value

void LbVariable_Print(LbVariable *ptr){
    if(ptr->type==VALUE_NUMBER)
        printf(LB_FLOAT_PRINT,ptr->var.number);
    else
        if(ptr->type==VALUE_STRING)
            printf("%s",ptr->var.string.str);
}
LbVariable* LbVariable_Alloc(){
    return (LbVariable*)malloc(sizeof(LbVariable));
}
void LbVariable_Init(LbVariable* ptr){
    ptr->type=VALUE_NUMBER;
    ptr->var.number=0;
}
LbVariable* LbVariable_Create(){
    LbVariable *out=LbVariable_Alloc();
    LbVariable_Init(out);
    return out;
}

void LbVariable_Dealloc(LbVariable* ptr){
    LbVariable_Clean(ptr);
    free(ptr);
}
void LbVariable_Clean(LbVariable* ptr){
    if(ptr->type==VALUE_STRING)
        LbString_Clean(&(ptr->var.string));
    LbVariable_Init(ptr);
}
void LbVariable_SetString(LbVariable *ptr,const char* string){
        LbVariable_Clean(ptr);
        LbString_Init(&(ptr->var.string),string);
        ptr->type=VALUE_STRING;
}
void LbVariable_SetNumber(LbVariable *ptr,LbFloat value){
        LbVariable_Clean(ptr);
        ptr->var.number=value;
        ptr->type=VALUE_NUMBER;
}
void LbVariable_SetFunction(LbVariable *ptr,int value){
        LbVariable_Clean(ptr);
		ptr->var.id=value;
        ptr->type=VALUE_FUNCTION;
}
void LbVariable_SetCFunction(LbVariable *ptr,int (*cfun) (VMliteB*)){
        LbVariable_Clean(ptr);
		ptr->var.cfun=cfun;
        ptr->type=VALUE_CFUNCTION;
}
LbVariable* LbVariable_Copy(LbVariable *ptr){

     LbVariable *out=LbVariable_Alloc();
     LbVariable_Replace(out,ptr);
     return out;

}
void LbVariable_Replace(LbVariable *in,LbVariable *ptr){

     LbVariable_Clean(in);
     in->type=ptr->type;

     if(ptr->type!=VALUE_STRING){
        in->var=ptr->var;
     }else{
        LbString_Init(&(in->var.string),ptr->var.string.str);
     }

}
//////////////////////////////OPERATOR
#define LBV_ISASTRING(v) ((v)->type==VALUE_STRING)
#define LBV_ISANUMBER(v) ((v)->type==VALUE_NUMBER)
uchar LbVariable_Cmd(LbVariable *ptr0,LbVariable *ptr1){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)){
            ptr0->var.number=-ptr1->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Add(LbVariable *ptr1,LbVariable *ptr2){
	    
		if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr1->var.number+=ptr2->var.number;
            return VALUE_NO_ERROR;
        }
       if(LBV_ISANUMBER(ptr1)){
           //set string
           char buffer[255];
           sprintf(buffer,LB_FLOAT_PRINT,ptr1->var.number);
		  //TO DO FIX
           LbVariable_SetString(ptr1,buffer);
           //
       }

       if(LBV_ISASTRING(ptr2))
                 LbString_AddString(&(ptr1->var.string),&(ptr2->var.string));

       if(LBV_ISANUMBER(ptr2)){
          char buffer[255];
          sprintf(buffer,LB_FLOAT_PRINT,ptr2->var.number);
          LbString tmp;
          tmp.str=buffer;
          tmp.len=strlen(buffer);
		  //TO DO FIX
          LbString_AddString(&(ptr1->var.string),&tmp);
       }

        return VALUE_NO_ERROR;
}
uchar LbVariable_Sub(LbVariable *ptr1,LbVariable *ptr2){
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr1->var.number-=ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Mul(LbVariable *ptr1,LbVariable *ptr2){
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr1->var.number*=ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Div(LbVariable *ptr1,LbVariable *ptr2){

        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr1->var.number/=ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Eq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){

        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number==ptr2->var.number;
        }
        else
        if(LBV_ISASTRING(ptr1)&&LBV_ISASTRING(ptr2)){
            ptr0->var.number=strcmp(ptr1->var.string.str,ptr2->var.string.str)==0;
        }
        else{
            ptr0->var.number=0;
        }

        return VALUE_NO_ERROR;
}
uchar LbVariable_GtEq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number>=ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_LtEq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){

        ptr0->var.number=0;

        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number<=ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;

}
uchar LbVariable_Gt(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number>ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Lt(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){

        ptr0->var.number=0;

        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number<ptr2->var.number;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;

}

uchar LbVariable_And(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number!=0.0 && ptr2->var.number!=0.0;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;

}
uchar LbVariable_Or(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)&&LBV_ISANUMBER(ptr2)){
            ptr0->var.number=ptr1->var.number!=0.0 || ptr2->var.number!=0.0;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
uchar LbVariable_Not(LbVariable *ptr0,LbVariable *ptr1){
        LbVariable_Clean(ptr0);
        if(LBV_ISANUMBER(ptr1)){
            ptr0->var.number=ptr1->var.number==0.0;
            return VALUE_NO_ERROR;
        }else return VALUE_AN_ERROR;
}
