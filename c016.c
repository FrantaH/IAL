
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

    int i = 0;
    while (i<HTSIZE) //naplnění pole ukazateli NULL
	{
        (*ptrht)[i] = NULL;
	i++;
	}

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

    int i = hashCode(key);      //zjištění na jaké pozici by se měl vyskytovat daný klíč
    tHTItem* tmp = (*ptrht)[i];
    if (tmp == NULL)        // pokud zde není nic
    {
        return NULL;

    } else if (tmp->key==key)   //pokud hned první synonymum je hledaný klíč
    {
        return tmp;
    } else {

        while (tmp->ptrnext!=NULL)  //projití ostatních synonym
        {
            if (tmp->ptrnext->key==key)
            {
                return tmp->ptrnext;
            } else
            tmp = tmp->ptrnext;
        }
        return NULL;
    }

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

    tHTItem* tmp = htSearch(ptrht,key);
    int i;
    if (tmp != NULL)        //pokud byl nalezen již vytvořený klíč
    {
        tmp->data = data;
    } else {
        i = hashCode(key);
        tmp = malloc(sizeof(tHTItem));

        if ((*ptrht)[i]==NULL)      //pokud je na pozici prázdno
        {
            (*ptrht)[i]=tmp;
            tmp->data=data;
            tmp->key=key;
            tmp->ptrnext=NULL;

        } else {            //pokud je zde namapován již jiný klíč
            tmp->data=data;
            tmp->key=key;
            tmp->ptrnext=(*ptrht)[i];
            (*ptrht)[i]=tmp;
        }
    }

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

    tHTItem* tmp = htSearch(ptrht,key);

    if (tmp == NULL)
        return NULL;
    else
        return &(tmp->data);

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

    int i = hashCode(key);
    tHTItem* tmp = (*ptrht)[i];
    if ((*ptrht)[i] != NULL)
    {
        if (tmp->key==key)
        {
            (*ptrht)[i] = tmp->ptrnext;
            free(tmp);
        } else {

            while (tmp->ptrnext != NULL)
            {
                if (tmp->ptrnext->key == key)
                {
                    tHTItem *swapper = tmp->ptrnext;
                    tmp->ptrnext = swapper->ptrnext;
                    free(swapper);
                    break;
                }
                tmp = tmp->ptrnext;
            }

        }
    }

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

    int i =0;
    tHTItem *tmp;
    tHTItem *deleting;

    while (i<HTSIZE)
    {
        tmp = (*ptrht)[i];
        while (tmp != NULL)
        {
            deleting = tmp;
            tmp = tmp->ptrnext;
            free(deleting);
        }
        i++;
    }

    htInit(ptrht);

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}
