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

}

/* subtraction: returns 0 if A < B */
BigNum* bn_sub(const BigNum* A, const BigNum* B) {

}

/* multiply one digit with shift */
static BigNum* bn_mul_single(const BigNum* A, int d, int shift) {

}

/* multiplication */
BigNum* bn_mul(const BigNum* A, const BigNum* B) {

}
