/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute       sortering
 ----------------------------------
    150		0,05		0,01
    200		0,1		0,025
    300		0,3		0,04
    400		0,7		0,065
    800		5,8		0,22
   1600		47		0,88
   3200		375		3,7
   6400		3000		16
  12800		24000		69


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(N⁴) Det finns 4 st nästlade for-loopar och allas längd är beroende på N. I värsta fall kommer den sista forloopen att köras varje gång
Första loopen körs ca N ggr. Andra loopen körs N/2 ggr. 3:e = N/4. 4:e = N/8.
Totalt = N*N/2*N/4*N/8=1/64*N⁴. Så O(N⁴)

Sortering: O(N²) Först en for-loop som går N ggr. Sedan en for-loop som går från i till N, och sedan en till som går lika långt. De inre for-looparna går då N/2.
Totalt = N*(N/2+N/2)=N². Så O(N²)
