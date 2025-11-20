#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bignum.h"

/* utility */
static Link* node_new(int d) {
    Link* n = malloc(sizeof(Link));
    n->digit = d;
    n->prev = n->next = NULL;
    return n;
}

BigNum* bn_from_string(const char* s) {
    if (!s)
        return NULL;
    BigNum* num = malloc(sizeof(BigNum));
    num->head = NULL;
    num->tail = NULL;
    while (*s == '0')
    {
        s++;
    }
    if(*s == '\0')
    {
        num->head = num->tail = node_new(0);
        return num;
    }
    while (*s != '\0')
    {
        int digit = *s - '0';
            Link* node = node_new(digit);
            if (num->head == NULL) {
                num->head = num->tail = node;
            }
            else {
                num->tail->next = node;
                node->prev = num->tail;
                num->tail = node;
            }
            s++;

    }

    return num;
}
char* bn_to_string(const BigNum* num){
        if (num->head == NULL)
        return NULL;
        int len=0;
        Link* p = num->head;
        while (p != NULL) {
            len++;
            p = p->next;
        }
        char* s = malloc((len + 1) * sizeof(char));
        p = num->head;
        int i = 0;
        while (p != NULL) {
            s[i] = '0' + p->digit;
            i++;
            p = p->next;
        }
        s[len] = '\0';
        return s;
}
/* compare */
int bn_compare(const BigNum* A, const BigNum* B) {
        Link* pA = A->head;
        Link* pB = B->head;
        int lenB=0,lenA = 0;
        while (pA != NULL) {
            lenA++;
            pA = pA->next;
        }
        while (pB != NULL) {
            lenB++;
            pB = pB->next;
        }
        pA = A->head;
        pB = B->head;
        if(lenA > lenB)
            return 1;
        if(lenB > lenA)
            return -1;
        if(lenA == lenB)
        {
            while (pA != NULL && pB != NULL) {
                if(pA->digit > pB->digit)
                   return 1;
                if(pB->digit > pA->digit)
                   return -1;
                pA = pA->next;
                pB = pB->next;
            }
        }
        return 0;
}

/* addition */
BigNum* bn_add(const BigNum* A, const BigNum* B) {
    BigNum* res = malloc (sizeof(BigNum));
    if (!res) return NULL;// checking for memory not null
    res->head = res->tail = NULL;//res equal null in the start
    Link* a = A->tail;
    Link* b = B->tail;
    int carry = 0;
    while (a != NULL || b != NULL||carry!=0) {
        int da,db;//digit a digit b
        if (a != NULL)
            da = a->digit;
        else
            da = 0;

        if (b != NULL)
            db = b->digit;
        else
            db = 0;

        int sum = da + db + carry;// calc sum of digits+carry
        int digit = sum % 10;
        carry = sum / 10;
        Link* node = node_new(digit);

        if (res->head == NULL) {//first digit of our result
            res->head = res->tail = node;
        } else {//next digits untill null
            node->next = res->head;
            res->head->prev = node;
            res->head = node;
        }
        //next digit to sum in a and b
        if (a != NULL) a = a->prev;
        if (b != NULL) b = b->prev;
    }

    return res;
}


/* subtraction */
BigNum* bn_sub(const BigNum* A, const BigNum* B) {
    if (bn_compare(A, B) < 0) {
        return bn_from_string("0");
    }
    BigNum* res = malloc(sizeof(BigNum));
    if (!res)
        return NULL;
    res->head = res->tail = NULL;//res equal null in the start
    Link* a = A->tail;
    Link* b = B->tail;
    int borrow = 0;
    while (a != NULL || b != NULL) {
        int da = 0;
        int db = 0;
        if (a != NULL)
            da = a->digit;
        else
            da = 0;
        if (b != NULL)
            db = b->digit;
        else
            db = 0;
        da = da - borrow;

        if (da < db) {
            da = da + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        int digit = da - db;
        Link* node = node_new(digit);

        if (res->head == NULL) {
            res->head = res->tail = node;
        } else {
            node->next = res->head;
            res->head->prev = node;
            res->head = node;
        }
        if (a != NULL) a = a->prev;
        if (b != NULL) b = b->prev;
    }
    //deletes zeros for example 000123 = 123
    Link* p = res->head;
    while (p != NULL && p->digit == 0 && p != res->tail) {
        Link* to_free = p;
        p = p->next;
        p->prev = NULL;
        res->head = p;
        free(to_free);
    }
    return res;

}
/* multiply one digit with shift */
BigNum* bn_mul_single(const BigNum* A, int d, int shift) {
    BigNum* res = malloc(sizeof(BigNum));
    res->head = res->tail = NULL;

    Link* a = A->tail;
    int carry = 0;

    while (a != NULL || carry != 0) {
        int da;
        if (a != NULL)
            da = a->digit;
        else
            da = 0;

        int prod = da * d + carry;
        int digit = prod % 10;
        carry = prod / 10;

        Link* node = node_new(digit);

        if (res->head == NULL) {
            res->head = res->tail = node;
        } else {
            node->next = res->head;
            res->head->prev = node;
            res->head = node;
        }

        if (a != NULL) a = a->prev;
    }

    for (int i = 0; i < shift; i++) {
        Link* zero = node_new(0);
        res->tail->next = zero;
        zero->prev = res->tail;
        res->tail = zero;
    }

    return res;
}

/* multiplication */
BigNum* bn_mul(const BigNum* A, const BigNum* B) {
    BigNum* result = bn_from_string("0");
    int shift = 0;

    for (Link* b = B->tail; b != NULL; b = b->prev) {
        BigNum* partial = bn_mul_single(A, b->digit, shift);

        BigNum* new_result = bn_add(result, partial);

        bn_free(result);
        bn_free(partial);

        result = new_result;
        shift++;
    }

    return result;
}


void bn_free(BigNum* a) {
    if (!a) return;

    Link* p = a->head;
    while (p != NULL) {
        Link* next = p->next;
        free(p);
        p = next;
    }

    free(a);
}
