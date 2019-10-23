#include <string.h>
#include <stdio.h>

#include "util.h"
#include "slp.h"
#include "answer.h"

typedef struct Table_ *Table;
typedef struct IntTablePair_ *IntTablePair;
typedef struct ResValPair_ *ResValPair;

#define null 0

struct Table_ {
    string key;
    int value;
    struct Table_ *tail;
};

struct IntTablePair_ {
    int i;
    Table t;
};

struct ResValPair_ {
    bool exist;
    int value;
};

int maxargsExp(A_exp exp, bool isPrint);
int maxargsExpList(A_expList exps);

Table interpStm(A_stm stm, Table t);
Table printExpList(A_expList exps, Table t);
IntTablePair interpExp(A_exp exp, Table t);

Table T_append(Table tail, string key, int value);
ResValPair T_get(Table t, string key);
Table T_update(Table t, string key, int value);

Table T_update(Table t, string key, int value) {
    if (t == null)
        return T_append(null, key, value);

    if (strcmp(key, t->key) == 0) {
        t->value = value;
        return t;
    }
    t->tail = T_update(t->tail, key, value);
    return t;
}

Table T_append(Table tail, string key, int value) {
    Table t = checked_malloc(sizeof(*t));
    t->key = key;
    t->value = value;
    t->tail = tail;
    return t;
}

ResValPair T_get(Table t, string key) {
    ResValPair pair = checked_malloc(sizeof(*pair));
    pair->exist = FALSE;
    if (t == null)
        return pair;
    if (strcmp(key, t->key) == 0) {
        pair->exist = TRUE;
        pair->value = t->value;
        return pair;
    }
    // 无视内存管理
    return T_get(t->tail, key);
}

int maxargsStm(A_stm stm) {
    if (stm == null) {
        return 0;
    }
    switch (stm->kind) {
        case A_compoundStm:
            return max(maxargsStm(stm->u.compound.stm1), maxargsStm(stm->u.compound.stm2));
        case A_assignStm:
            return maxargsExp(stm->u.assign.exp, FALSE);
        case A_printStm:
            return maxargsExpList(stm->u.print.exps);
    }
    return 0;
}

int maxargsExpList(A_expList exps) {
    if (exps == null) {
        return 0;
    }

    switch (exps->kind) {
        case A_lastExpList:
            return 1;
        case A_pairExpList:
            return maxargsExp(exps->u.pair.head, TRUE) + maxargsExpList(exps->u.pair.tail);
        default:
            return 0;
    }
}

int maxargsExp(A_exp exp, bool isPrint) {
    if (exp == null)
        return 0;

    switch (exp->kind) {
        case A_idExp:
        case A_numExp:
        case A_opExp:
            if (isPrint)
                return 1;
            return 0;
        case A_eseqExp:
            return max(maxargsStm(exp->u.eseq.stm), maxargsExp(exp->u.eseq.exp, FALSE));
        default:
            return 0;
    }
}

Table interpStm(A_stm stm, Table t) {
    if (stm == null)
        return t;

    IntTablePair expRes;
    switch (stm->kind) {
        case A_compoundStm:
            t = interpStm(stm->u.compound.stm1, t);
            t = interpStm(stm->u.compound.stm2, t);
            break;
        case A_assignStm:
            expRes = interpExp(stm->u.assign.exp, t);
            t = T_update(expRes->t, stm->u.assign.id, expRes->i);
            break;
        case A_printStm:
            t = printExpList(stm->u.print.exps, t);
        default:
            break;
    }
    return t;
}

IntTablePair interpExp(A_exp exp, Table t) {
    IntTablePair intTablePair = checked_malloc(sizeof(*intTablePair));
    intTablePair->t = t;
    if (exp == null)
        return intTablePair;

    ResValPair res;
    IntTablePair leftRes;
    IntTablePair rightRes;
    int val;
    switch (exp->kind) {
        case A_idExp:
            res = T_get(t, exp->u.id);
            if (res->exist)
                intTablePair->i = res->value;
            else
                printf("[Error] Identifier %s does not exist!\n", exp->u.id);
            return intTablePair;
        case A_numExp:
            intTablePair->i = exp->u.num;
            return intTablePair;
        case A_opExp:
            leftRes = interpExp(exp->u.op.left, t);
            rightRes = interpExp(exp->u.op.right, leftRes->t);
            switch (exp->u.op.oper) {
                case A_plus:
                    val = leftRes->i + rightRes->i;
                    break;
                case A_minus:
                    val = leftRes->i - rightRes->i;
                    break;
                case A_times:
                    val = leftRes->i * rightRes->i;
                    break;
                case A_div:
                    val = leftRes->i / rightRes->i;
                    break;
            }
            intTablePair->i = val;
            intTablePair->t = rightRes->t;
            return intTablePair;
        case A_eseqExp:
            t = interpStm(exp->u.eseq.stm, t);
            return interpExp(exp->u.eseq.exp, t);
        default:
            return intTablePair;
    }
}

Table printExpList(A_expList exps, Table t) {
    if (exps == null) {
        printf("\n");
        return t;
    }

    A_exp exp;
    A_expList tail = null;
    if (exps->kind == A_pairExpList) {
        exp = exps->u.pair.head;
        tail = exps->u.pair.tail;
    }
    else
        exp = exps->u.last;

    IntTablePair expRes = interpExp(exp, t);
    printf("%d ", expRes->i);
    return printExpList(tail, expRes->t);
}

void interpWrapper(A_stm stm) {
    interpStm(stm, null);
}