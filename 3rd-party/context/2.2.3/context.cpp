/*
  C++ port of Context 2.07
  Author: unknown
*/

#include  <io.h>
#include  <fcntl.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <stdlib.h>

extern  "C" int puts( const char *string );

typedef  unsigned char byte;
typedef  unsigned int  word;

#define  eNOFILE         "Невозможно откpыть файл"          // Can't open file
#define  eEOF            "Конец файла"                      // Unexpected EOF
#define  eNOMEMORY       "Недостаточно памяти"              // Not enough memory
#define  eWRONGCHAR      "Недопустимый символ в константе"  // Illegal character in const
#define  eOVERFLOW       "Константа слишком велика"         // Too big number
#define  eLONGNAME       "Слишком длинный идентификатоp"    // Name length too long
#define  eEXPECTED       "Пpопущен идентификатоp"           // Identifier expected
#define  eUNDEFINED      "Идентификатоp не опpеделен"       // Undefined identifier
#define  eDUPLICATE      "Повтоp имени"                     // Duplicate identifier
#define  eBRACKETEXP     "Пpопущена скобка"                 // Bracket expected
#define  eCASEEXP        "Пpопущено case"                   // case expected
#define  eTHENEXP        "Пpопущено then или двоеточие"     // then or colon expected
#define  eCOLONEXP       "Пpопущено двоеточие"              // Colon expected
#define  eDOEXP          "Пpопущено do"                     // do expected
#define  eWHILEEXP       "loop/exit вне цикла"              // loop/exit without cycle
#define  eSTRUCTEXP      "Пpопущена стpуктуpная пеpеменная" // Struct variable expected
#define  eQUOTEXP        "Пpопущена кавычка"                // Quote expected
#define  eSTRING         "Символ @ недопустим"              // Illegal use of @
#define  eNOADDR         "Невозможно вычислить адpес"       // Can't calculate address
#define  eOPERATOR       "Два опеpатоpа подpяд"             // Two operators togeter
#define  eTYPE           "Несоответствие типов"             // Incompatible types
#define  eTYPEEXP        "Пpопущен идентификатоp типа"      // Type name expected
#define  eNOVOID         "Тип void недопустим"              // Illegal use of void
#define  eNOFUNCTION     "Объявление функции недопустимо"   // Illegal function definition
#define  eASSIGNEXP      "Пpопущено ="                      // = expected
#define  eSEMICOLONEXP   "Пpопущена точка с запятой"        // Semicolon expected
#define  eCOMMAEXP       "Пpопущена запятая"                // Comma expected
#define  eNOTAVAIL       "Не pеализовано"                   // Feature not implemented
#define  eINTERNAL       "Внутpенняя ошибка"                // Internal error

#define  nNODE    32768
#define  nCHAR     8192
#define  nTEXT     4096
#define  nDICT     1024
#define  nPARM     1024
#define  nINDX      512
#define  nNAME       32
#define  nFILE        8
#define  nBUFF      128
#define  nINFO       32
#define  cWORD        0
#define  cNAME        1
#define  cCONST       2
#define  cTYPE        3
#define  cDATA        4
#define  cFUNC        5
#define  cLIB         6
#define  cFILE        7
#define  sBASE        0
#define  sSTRUCT      1
#define  sSYS         0
#define  sFUNC        1
#define  sMAIN        2
#define  iEMPTY       0
#define  iNULL        1
#define  iCHAR        2
#define  iWORD        3
#define  iINT         4
#define  iFUNC        5
#define  iSTRING      6
#define  iDATA        7
#define  iPARM        8
#define  iLOCAL       9
#define  iREF        10
#define  iNOT        11
#define  iNEG        12
#define  iOR         13
#define  iXOR        14
#define  iEQV        15
#define  iAND        16
#define  iBIT_OR     17
#define  iBIT_XOR    18
#define  iBIT_AND    19
#define  iLT         20
#define  iLE         21
#define  iEQ         22
#define  iNE         23
#define  iGE         24
#define  iGT         25
#define  iADD        26
#define  iSUB        27
#define  iMUL        28
#define  iDIV        29
#define  iMOD        30
#define  iCALL       31
#define  iINDEX      32
#define  iADDR       33
#define  iFIELD      34
#define  iLOAD       35
#define  iLPTR       36
#define  iDEF        37
#define  iSELECT     38
#define  iWHILE      39
#define  iLOOP       40
#define  iEXIT       41
#define  iCAST       42
#define  iINC        43
#define  iDEC        44
#define  iPUSH       45
#define  iASSIGN     46
#define  iRETURN     47
#define  pZERO        0
#define  pBOOL        1
#define  pCOMP        2
#define  pADD         3
#define  pMUL         4
#define  pINC         5
#define  pLVALUE      6

struct NODE
{
  word ID;
  word Value;
  word pLeft;
  word pRight;
};

struct DICT
{
  char Name [ nNAME ];
  word Class;
  word Sub;
  word pType;
  word nPtr;
  word pIndx;
  word nIndx;
  word pNode;
  word Value;
  word Flag;
};

struct PARM
{
  char Name [ nNAME ];
  word pType;
  word nPtr;
  word pIndx;
  word nIndx;
  word Value;
};

struct FILE
{
  char Info [ nNAME ];
  word hText;
  word nText;
  word pText;
  word nLine;
};

DICT Dict [ nDICT ];
word nDict;
PARM Parm [ nPARM ];
word nParm;
word Indx [ nINDX ];
word nIndx;
char Char [ nCHAR ];
word nChar;
NODE Node [ nNODE ];
word nNode;
FILE File [ nFILE ];
word nFile;
word pFile;
char Text [ 32768 ];
word nText;
word pText;
char Info [ nINFO ];
word hText;
word nLine;
word isalpha( char Ch )
{
/*switch ( Ch ) {
  case  '`':
  case  '1':
  case  '2':
  case  '3':
  case  '4':
  case  '5':
  case  '6':
  case  '7':
  case  '8':
  case  '9':
  case  '0':
  case  '-':
  case  '=':
  case  '\\':
  case  '[':
  case  ']':
  case  ';':
  case  '\'':
  case  ',':
  case  '.':
  case  '/':
  case  '~':
  case  '!':
  case  '@':
  case  '#':
  case  '$':
  case  '%':
  case  '^':
  case  '&':
  case  '*':
  case  '(':
  case  ')':
  case  '+':
  case  '|':
  case  '{':
  case  '}':
  case  ':':
  case  '"':
  case  '<':
  case  '>':
  case  '?':
  case  ' ':
  case  '\a':
  case  '\b':
  case  '\f':
  case  '\n':
  case  '\r':
  case  '\t':
  case  '\v':
    return 1;
  }
  return 0;*/
  if ( ( 'A' <= Ch && Ch <= 'Z' ) || ( 'a' <= Ch && Ch <= 'z' ) || ( Ch == '_' ) ) {
    return 0;
  }
  return 1;
}
word isdigit( char Ch )
{
  if ( ( '0' <= Ch && Ch <= '9' ) || ( Ch == '$' ) ) {
    return 0;
  }
  return 1;
}
word strlen( char *St )
{
  word P = 0;
  while ( St[ P ] != 0 ) {
    ++ P;
  }
  return P;
}
word strcmp( char *St1, char *St2 )
{
  word P = 0;
  while ( St1[ P ] == St2[ P ] ) {
    if ( St1[ P ] == 0 ) {
      return 0;
    }
    ++ P;
  }
  return 1;
}
char *strcpy( char *Dst, char *Src )
{
  word P = 0;
  while ( Src[ P ] != 0 ) {
    Dst[ P ] = Src[ P ];
    ++ P;
  }
  Dst[ P ] = 0;
  return Dst;
}
char *strcat( char *Dst, char *Src )
{
  strcpy( &Dst[ strlen( Dst ) ], Src );
  return Dst;
}
char *str( word N )
{
  static  char Buff[ 11 ];
  strcpy( Buff, "0000000000" );

  char *Digit = "0123456789";
  word I = 0;
  while ( N > 0 || I == 0 ) {
    Buff[ I ] = Digit[ N % 10 ];
    ++ I;
    N = N / 10;
  }
  Buff[ I ] = 0;
  word J = 0;
  word K = I - 1;
  while ( J < K ) {
    char C = Buff[ J ];
    Buff[ J ] = Buff[ K ];
    Buff[ K ] = C;
    -- K;
    ++ J;
  }
  return Buff;
}
word create( char *Name )
{
//return _open( Name, _O_WRONLY | _O_CREAT | _O_TEXT | _O_TRUNC, _S_IREAD | _S_IWRITE );
  return  open( Name, _O_WRONLY | _O_CREAT | _O_TEXT | _O_TRUNC, _S_IWRITE);
}
word open( char *Name )
{
  return _open( Name, _O_RDONLY );
}
word read( word F, void *Buff, word N )
{
  return _read( F, Buff, N );
}
word write( word F, void *Buff, word N )
{
  return _write( F, Buff, N );
}
void close( word F )
{
  _close( F );
}
void Stop( char *Msg )
{
  char Buff[ nBUFF ];
  puts ( strcat( strcat( strcat( strcat( strcat( strcat( strcpy( Buff, "\r" ), Info ), "(" ), str( nLine ) ), "): " ), Msg ), "\r\n" ) );
  close ( hText );
  while ( nFile > 0 ) {
    -- nFile;
    close( File[ nFile ].hText );
  }
  exit( 1 );
}
word val( char *Buff )
{
  char * D = "0123456789ABCDEF";
  word W = 0x10000;
  word E = 10;
  word P = 0;
  if ( Buff[ 0 ] == '$' ) {
    E = 16;
    P = 1;
  }
  word L = 0;
  word H = 0;
  while ( Buff[ P ] != 0 ) {
    word S = 0;
    while ( D[ S ] != Buff[ P ] ) {
      ++ S;
      if ( S >= E ) {
        Stop( eWRONGCHAR );
      }
    }
    S = E * L + S;
    L = S % W;
    S = E * H + S / W;
    H = S % W;
    S = S / W;
    if ( S > 0 ) {
      Stop( eOVERFLOW );
    }
    ++ P;
  }
  return W*H + L;
}
void Copy( char *Dest, char *Name, word P )
{
  while ( P < nDict && Dict[ P ].Class == cWORD ) {
    if ( strcmp( Dict[ P ].Name, Name ) == 0 ) {
      Stop( eEXPECTED );
    }
    ++ P;
  }
  if ( isalpha( Name[ 0 ] ) != 0 ) {
    Stop( eEXPECTED );
  }
  strcpy( Dest, Name );
}
word Find( char *Name )
{
  word P = 0;
  while ( P < nDict ) {
    if ( strcmp( Dict[ P ].Name, Name ) == 0 ) {
      while ( Dict[ P ].Class == cNAME ) {
        P = Dict[ P ].pType;
      }
      break; //exit
    }
    ++ P;
  }
  return P;
}
void Word( char *Name )
{
  Dict[ nDict ].Class = cWORD;
  strcpy( Dict[ nDict ].Name, Name );
  Dict[ nDict ].Flag = 0;
  ++ nDict;
}
void Base( char *Name, word Size )
{
  Dict[ nDict ].Class = cTYPE;
  strcpy( Dict[ nDict ].Name, Name );
  Dict[ nDict ].Sub = sBASE;
  Dict[ nDict ].Value = Size;
  Dict[ nDict ].Flag = 0;
  ++ nDict;
}
char Read()
{
  word Flag = 0;
  while ( pText >= nText ) {
    if ( Flag == 0 ) {
      nText = pFile + read( hText, &Text[ pFile ], nTEXT );
      pText = pFile;
      Flag = 1;
    }
    else {
      if ( nFile < 1 ) {
        Stop( eEOF );
      }
      char Temp[ nBUFF ];
      puts ( strcat( strcat( strcat( strcat( strcpy( Temp, "\r" ), Info ), "(" ), str( nLine ) ), ")\r\n" ) );
      close( hText );
      -- nFile;
      hText = File[ nFile ].hText;
      strcpy( Info, File[ nFile ].Info );
      nText = File[ nFile ].nText;
      pText = File[ nFile ].pText;
      nLine = File[ nFile ].nLine;
      pFile = pFile - nTEXT;
      Flag = 0;
    }
  }
  return Text[ pText ];
}
void Next()
{
  ++ pText;
}
char Sign()
{
  char Ch = Read();
  Next();
  if ( Ch == '~' ) {
    Ch = Read();
    //{//select
    if ( Ch == 'n' ) {
      Ch = '\n';
    }
    else if ( Ch == 'r' ) {
      Ch = '\r';
    }
    else if ( Ch == 't' ) {
      Ch = '\t';
    }
    else if ( Ch == '0' ) {
      Ch = '\0';
    }
    Next();
  }
  return Ch;
}
void Line()
{
  Dict[ nDict ].pType = Find( "char" );
  Dict[ nDict ].nPtr = 1;
  Dict[ nDict ].pIndx = nChar;
  while ( Read() != '"' ) {
    if ( Read() == '\r' || Read() == '\n' ) {
      Stop( eQUOTEXP );
    }
    if ( nChar >= nCHAR ) {
      Stop( eNOMEMORY );
    }
    Char[ nChar ] = Sign();
    ++ nChar;
  }
  Next();
  if ( nChar >= nCHAR ) {
    Stop( eNOMEMORY );
  }
  Char[ nChar ] = 0;
  ++ nChar;
  Dict[ nDict ].nIndx = nChar;
}
char *Scan( char *Buff )
{
  word N = 0;
  do {
    while ( Read() == '\n' || Read() == '\r' || Read() == '\t' || Read() == ' ' ) {
      if ( Read() == '\n' ) {
        if ( nLine % 100 == 0 ) {
        //puts( strcat( strcat( strcat( strcat( strcpy( Buff, "\r" ), Info ), "(" ), str( nLine ) ), ")" ) );
        }
        ++ nLine;
      }
      Next();
    }
    if ( Read() == '/' ) {
      Next();
      if ( Read() == '*' ) {
        Next();
        ++ N;
      }
      if ( N > 0 ) {
        continue; //loop
      }
      if ( Read() == '/' ) {
        while ( Read() != '\n' ) {
          Next();
        }
        continue; //loop
      }
      return strcpy( Buff, "/" );
    }
    if ( N == 0 ) {
      break; //exit
    }
    if ( Read() == '*' ) {
      Next();
      if ( Read() == '/' ) {
        Next();
        -- N;
      }
      continue; // 03.01.2005 // break; //loop
    }
    Next();
  }
  while ( 1 );
  word P = 0;
  while ( isalpha( Read() ) == 0 || isdigit( Read() ) == 0 ) {
    Buff[ P ] = Read();
    ++ P;
    if ( P >= nNAME ) {
      Stop( eLONGNAME );
    }
    Next();
  }
  if ( P == 0 ) {
    Buff[ P ] = Read();
    ++ P;
    Next();
    //{//select
    if ( Buff[ 0 ] == '!' ) {
      if ( Read() == '=' ) {
        Next();
        return strcpy( Buff, "!=" );
      }
    }
    else if ( Buff[ 0 ] == '<' ) {
      if ( Read() == '=' ) {
        Next();
        return strcpy( Buff, "<=" );
      }
    }
    else if ( Buff[ 0 ] == '>' ) {
      if ( Read() == '=' ) {
        Next();
        return strcpy( Buff, ">=" );
      }
    }
  }
  Buff[ P ] = 0;
  return Buff;
}
word Tabl( char *Buff )
{
  while ( strcmp( Buff, "[" ) == 0 ) {
    if ( nIndx >= nINDX ) {
      Stop( eNOMEMORY );
    }
    word P = Find( Scan( Buff ) );
    if ( P < nDict && Dict[ P ].Class == cCONST && Dict[ P ].pType == Find( "word" ) ) {
      Indx[ nIndx ] = Dict[ P ].Value;
    }
    else {
      Indx[ nIndx ] = val( Buff );
    }
    ++ nIndx;
    if ( strcmp( Scan( Buff ), "]" ) != 0 ) {
      Stop( eBRACKETEXP );
    }
    Scan( Buff );
  }
  return nIndx;
}
word List( char *Buff )
{
  word P = nParm;
  while ( strcmp( Buff, ")" ) != 0 ) {
    word pType = Find( Buff );
    if ( pType >= nDict || Dict[ pType ].Class != cTYPE ) {
      Stop( eTYPEEXP );
    }
    do {
      word nPtr = 0;
      while ( strcmp( Scan( Buff ), "@" ) == 0 ) {
        ++ nPtr;
      }
      if ( pType == Find( "void" ) && nPtr < 1 ) {
        Stop( eNOVOID );
      }
      word I = P;
      while ( I < nParm ) {
        if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
          Stop( eDUPLICATE );
        }
        ++ I;
      }
      if ( nParm >= nPARM ) {
        Stop( eNOMEMORY );
      }
      Parm[ nParm ].pType = pType;
      Copy( Parm[ nParm ].Name, Buff, 0 );
      Parm[ nParm ].nPtr = nPtr;
      ++ nParm;
      if ( strcmp( Scan( Buff ), "," ) != 0 ) {
        break; //exit
      }
    }
    while ( 1 );
    if ( strcmp( Buff, ")" ) != 0 ) {
      if ( strcmp( Buff, ";" ) != 0 ) {
        Stop( eSEMICOLONEXP );
      }
      if ( strcmp( Scan( Buff ), ")" ) == 0 ) {
        Stop( eTYPEEXP );
      }
    }
  }
  return nParm;
}
word Peek()
{
  word N = nNode;
  if ( N >= nNODE ) {
    Stop( eNOMEMORY );
  }
  Node [ nNode ].pLeft = nNODE;
  Node [ nNode ].pRight = nNODE;
  ++ nNode;
  return N;
}
void Cast( word pType1, word pType2, word &P )
{
  if ( pType1 == Find( "int" ) && Node[ P ].ID == iWORD ) {
    Node[ P ].ID = iINT;
    return ;
  }
  word P1 = Peek();
  word P2 = Peek();
  Node[ P1 ]. ID = iCAST;
  Node[ P1 ]. Value = pType1;
  Node[ P1 ].pLeft = P2;
  Node[ P2 ]. ID = iEMPTY;
  Node[ P2 ].pLeft = P;
  P = P1;
}
void Test( word pType1, word nPtr1, word pType2, word nPtr2, word &P )
{
  //{//select
  if ( pType1 == pType2 && nPtr1 == nPtr2 ) {
    return ;
  }
  else if ( pType1 == Find( "byte" ) && nPtr1 == 0 && pType2 == Find( "word" ) && nPtr2 == 0 ) {
    return ;
  }
  else if ( pType1 == Find( "byte" ) && nPtr1 == 0 && pType2 == Find( "int" ) && nPtr2 == 0 ) {
    return ;
  }
  else if ( pType1 == Find( "word" ) && nPtr1 == 0 && pType2 == Find( "byte" ) && nPtr2 == 0 ) {
    Cast( pType1, pType2, P );
    return ;
  }
  else if ( pType1 == Find( "word" ) && nPtr1 == 0 && pType2 == Find( "int" ) && nPtr2 == 0 ) {
    return ;
  }
  else if ( pType1 == Find( "int" ) && nPtr1 == 0 && pType2 == Find( "byte" ) && nPtr2 == 0 ) {
    Cast( pType1, pType2, P );
    return ;
  }
  else if ( pType1 == Find( "int" ) && nPtr1 == 0 && pType2 == Find( "word" ) && nPtr2 == 0 ) {
    return ;
  }
  else if ( pType1 == Find( "void" ) && ( ( nPtr1 > 0 && nPtr2 == nPtr1 ) || ( nPtr1 == 1 && nPtr2 > 0 ) ) ) {
    return ;
  }
  else if ( pType2 == Find( "void" ) && ( ( nPtr2 > 0 && nPtr1 == nPtr2 ) || ( nPtr2 == 1 && nPtr1 > 0 ) ) ) {
    return ;
  }
  Stop( eTYPE );
}
void Not ( word P )
{
  //{//select
  if ( Node[ P ].ID == iOR ) {
    Node[ P ].ID = iAND;
    Not ( Node[ P ].pLeft );
    Not ( Node[ P ].pRight );
  }
  else if ( Node[ P ].ID == iXOR ) {
    Node[ P ].ID = iEQV;
  }
  else if ( Node[ P ].ID == iEQV ) {
    Node[ P ].ID = iXOR;
  }
  else if ( Node[ P ].ID == iAND ) {
    Node[ P ].ID = iOR;
    Not ( Node[ P ].pLeft );
    Not ( Node[ P ].pRight );
  }
  else if ( Node[ P ].ID == iLT ) {
    Node[ P ].ID = iGE;
  }
  else if ( Node[ P ].ID == iLE ) {
    Node[ P ].ID = iGT;
  }
  else if ( Node[ P ].ID == iEQ ) {
    Node[ P ].ID = iNE;
  }
  else if ( Node[ P ].ID == iNE ) {
    Node[ P ].ID = iEQ;
  }
  else if ( Node[ P ].ID == iGE ) {
    Node[ P ].ID = iLT;
  }
  else if ( Node[ P ].ID == iGT ) {
    Node[ P ].ID = iLE;
  }
  else { //default
    Stop( eINTERNAL );
  }
}
word Dup ( word P )
{
  if ( P >= nNODE ) {
    return P;
  }
  word P1 = Peek();
  Node[ P1 ].ID = Node[ P ].ID;
  Node[ P1 ].Value = Node[ P ].Value;
  Node[ P1 ].pLeft = Dup ( Node[ P ].pLeft );
  Node[ P1 ].pRight = Dup ( Node[ P ].pRight );
  return P1;
}
word pFunc;
word pParm;
word pIndx;
word pLoop;
word Expr( word Prty, word &pType, word &nPtr, char *Buff )
{
  word Flag = 0;
  word Call = 0;
  word P1;
  //{//select
  if ( strcmp( Buff, "(" ) == 0 ) {
    P1 = Expr( pZERO, pType, nPtr, Scan( Buff ) );
    if ( strcmp( Buff, ")" ) != 0 ) {
      Stop( eBRACKETEXP );
    }
  }
  else if ( strcmp( Buff, "!" ) == 0 ) {
    if ( Prty > pBOOL ) {
      Stop( eOPERATOR );
    }
    word P2 = Expr( pMUL, pType, nPtr, Scan( Buff ) );
    if ( pType < nDICT && nPtr > 0 ) {
      Stop( eTYPE );
    }
    //{//select
    if ( pType == Find( "byte" ) || pType == Find( "word" ) ) {
      P1 = Peek();
      Node[ P1 ].ID = iNOT;
      Node[ P1 ].pLeft = P2;
    }
    else if ( pType >= nDICT ) {
      Not( P2 );
      P1 = P2;
    }
    else { //default
      Stop( eTYPE );
    }
    Flag = 1;
  }
  else if ( strcmp( Buff, "-" ) == 0 ) {
    if ( Prty > pCOMP ) {
      Stop( eOPERATOR );
    }
    P1 = Peek();
    Node[ P1 ]. ID = iNEG;
    Node[ P1 ].pLeft = Expr( pMUL, pType, nPtr, Scan( Buff ) );
    Test( Find( "int" ), 0, pType, nPtr, Node[ P1 ].pLeft );
    pType = Find( "int" );
    Flag = 1;
  }
  else if ( strcmp( Buff, "#" ) == 0 ) {
    word N = val( Scan( Buff ) );
    if ( N > 255 ) {
      Stop( eOVERFLOW );
    }
    P1 = Peek();
    Node[ P1 ].ID = iCHAR;
    Node[ P1 ].Value = N;
    pType = Find( "char" );
    nPtr = 0;
  }
  else if ( strcmp( Buff, "'" ) == 0 ) {
    P1 = Peek();
    Node[ P1 ].ID = iCHAR;
    Node[ P1 ].Value = word( Sign() );
    pType = Find( "char" );
    nPtr = 0;
    if ( Read() != '\'' ) {
      Stop( eQUOTEXP );
    }
    Next();
  }
  else if ( strcmp( Buff, "NULL" ) == 0 ) {
    P1 = Peek();
    Node[ P1 ].ID = iNULL;
    pType = Find( "void" );
    nPtr = 1;
  }
  else if ( isdigit( Buff[ 0 ] ) == 0 ) {
    P1 = Peek();
    Node[ P1 ].ID = iWORD;
    Node[ P1 ].Value = val ( Buff );
    pType = Find( "word" );
    nPtr = 0;
  }
  else { //default
    nPtr = 0;
    while ( strcmp( Buff, "@" ) == 0 ) {
      ++ nPtr;
      Scan( Buff );
    }
    word nPtr1 = 0;
    word pIndx = 0;
    word nIndx = 0;
    if ( Flag == 0 ) {
      if ( strcmp( Buff, "\"" ) == 0 ) {
        if ( nPtr > 0 ) {
          Stop( eSTRING );
        }
        nPtr = 1;
        if ( nDict >= nDICT ) {
          Stop( eNOMEMORY );
        }
        Dict[ nDict ].Class = cCONST; strcpy( Dict[ nDict ].Name, "" );
        Dict[ nDict ].Flag = 0;
        Line();
        P1 = Peek();
        Node[ P1 ].ID = iSTRING;
        Node[ P1 ].Value = nDict;
        pType = Find( "char" );
        Flag = 1;
        ++ nDict;
      }
    }
    if ( Flag == 0 ) {
      word I = pParm;
      while ( I < nParm ) {
        if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
          P1 = Peek();
          Node[ P1 ].ID = iLOCAL;
          Node[ P1 ].Value = I - pParm;
          pType = Parm[ I ].pType;
          nPtr1 = Parm[ I ].nPtr;
          pIndx = Parm[ I ].pIndx;
          nIndx = Parm[ I ].nIndx;
          Flag = 1;
          break; //exit
        }
        ++ I;
      }
    }
    if ( Flag == 0 ) {
      word I = Dict[ pFunc ].pIndx;
      while ( I < Dict[ pFunc ].nIndx ) {
        if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
          P1 = Peek();
          Node[ P1 ].ID = iPARM;
          Node[ P1 ].Value = I;
          pType = Parm[ I ].pType;
          nPtr1 = Parm[ I ].nPtr;
          Flag = 1;
          break; //exit
        }
        ++ I;
      }
    }
    if ( Flag == 0 ) {
      word I = Find( Buff );
      if ( I > nDict ) {
        Stop( eUNDEFINED );
      }
      //{//select
      if ( Dict[ I ].Class == cCONST ) {
        P1 = Peek();
        //{//select
        if ( Dict[ I ].pType == Find( "word" ) ) {
          Node[ P1 ].ID = iWORD;
          Node[ P1 ].Value = Dict[ I ].Value;
        }
        else if ( Dict[ I ].nPtr == 0 ) {
          Node[ P1 ].ID = iCHAR;
          Node[ P1 ].Value = Dict[ I ].Value;
        }
        else { //default
          Node[ P1 ].ID = iSTRING;
          Node[ P1 ].Value = I;
        }
        pType = Dict[ I ].pType;
      }
      else if ( Dict[ I ].Class == cDATA || Dict[ I ].Class == cFUNC ) {
        P1 = Peek();
        Node[ P1 ].ID = iDATA;
        Node[ P1 ].Value = I;
        pType = Dict[ I ].pType;
        nPtr1 = Dict[ I ].nPtr;
        if ( Dict[ I ].Class == cFUNC ) {
          if ( strcmp( Scan( Buff ), "(" ) == 0 ) {
            word P2 = Peek();
            Node[ P1 ].pLeft = P2;
            Node[ P2 ].ID = iCALL;
            Node[ P2 ].Value = I;
            Call = 1;
            word *P3 = &Node[ P2 ].pLeft;
            Scan( Buff );
            word pParm = Dict[ I ].pIndx;
            while ( pParm < Dict[ I ].nIndx ) {
              word pType2, nPtr2;
              word P4 = Peek();
              Node[ P4 ].ID = iPUSH;
              Node[ P4 ].pLeft = Expr( pZERO, pType2, nPtr2, Buff );
              Test( Parm[ pParm ].pType, Parm[ pParm ].nPtr, pType2, nPtr2, Node[ P4 ].pLeft );
              *P3 = P4;
              P3 = &Node[ P4 ].pRight;
              ++ pParm;
              if ( pParm < Dict[ I ].nIndx ) {
                if ( strcmp( Buff, "," ) != 0 ) {
                  Stop( eCOMMAEXP );
                }
                Scan( Buff );
              }
            }
            if ( strcmp( Buff, ")" ) != 0 ) {
              Stop( eBRACKETEXP );
            }
          }
          else {
            if ( Prty >= pINC || nPtr != 1 ) {
              Stop( eNOADDR );
            }
            Node[ P1 ].ID = iFUNC;
            Node[ P1 ].Value = I;
            pType = Find( "void" );
            return P1;
          }
        }
        else {
          pIndx = Dict[ I ].pIndx;
          nIndx = Dict[ I ].nIndx;
        }
      }
      else if ( Dict[ I ].Class == cTYPE ) {
        if ( Prty < pLVALUE ) {
          if ( strcmp( Scan( Buff ), "(" ) != 0 ) {
            Stop( eBRACKETEXP );
          }
          P1 = Peek();
          word P2 = Peek();
          Node[ P1 ]. ID = iCAST;
          Node[ P1 ]. Value = I;
          Node[ P1 ].pLeft = P2;
          Node[ P2 ]. ID = iEMPTY;
          Node[ P2 ].pLeft = Expr( pZERO, pType, nPtr1, Scan( Buff ) );
          if ( strcmp( Buff, ")" ) != 0 ) {
            Stop( eBRACKETEXP );
          }
          //{//select
          if ( I == Find( "char" ) && nPtr == 0 && pType == Find( "byte" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else if ( I == Find( "char" ) && nPtr == 0 && pType == Find( "word" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else if ( I == Find( "char" ) && nPtr == 0 && pType == Find( "int" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else if ( I == Find( "byte" ) && nPtr == 0 && pType == Find( "char" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else if ( I == Find( "word" ) && nPtr == 0 && pType == Find( "char" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else if ( I == Find( "int" ) && nPtr == 0 && pType == Find( "char" ) && nPtr1 == 0 ) {
            pType = I;
          }
          else { //default
            Stop( eTYPE );
          }
        }
        else {
          if ( nPtr > 0 ) {
            Stop( eUNDEFINED );
          }
          return nNODE;
        }
      }
      else { //default
        Stop( eUNDEFINED );
      }
    }
    word *P2 = &Node[ P1 ].pLeft;
    if ( Node[ P1 ].pLeft < nNODE ) {
      P2 = &Node[ Node[ P1 ].pLeft ].pRight;
    }
    do {
      Scan( Buff );
      word pIndx1 = pIndx;
      while ( pIndx < nIndx ) {
        if ( strcmp( Buff, "[" ) != 0 ) {
          if ( pIndx != pIndx1 || strcmp( Buff, "." ) == 0 ) {
            Stop( eBRACKETEXP );
          }
          break; //exit
        }
        word pType2, nPtr2;
        word P3 = Peek();
        Node[ P3 ].ID = iINDEX;
        Node[ P3 ].Value = Indx[ pIndx ];
        Node[ P3 ].pLeft = Expr( pZERO, pType2, nPtr2, Scan( Buff ) );
        if ( strcmp( Buff, "]" ) != 0 ) {
          Stop( eBRACKETEXP );
        }
        Test( Find( "word" ), 0, pType2, nPtr2, Node[ P3 ].pLeft );
        *P2 = P3;
        P2 = &Node[ P3 ].pRight;
        Scan( Buff );
        ++ pIndx;
      }
      while ( strcmp( Buff, "[" ) == 0 ) {
        if ( nPtr1 < 1 ) {
          Stop( eNOADDR );
        }
        word pType2, nPtr2;
        word P3 = Peek();
        Node[ P3 ].ID = iADDR;
        Node[ P3 ].pLeft = Expr( pZERO, pType2, nPtr2, Scan( Buff ) );
        if ( strcmp( Buff, "]" ) != 0 ) {
          Stop( eBRACKETEXP );
        }
        Test( Find( "word" ), 0, pType2, nPtr2, Node[ P3 ].pLeft );
        *P2 = P3;
        P2 = &Node[ P3 ].pRight;
        -- nPtr1;
        Call = 0;
        Scan( Buff );
      }
      if ( strcmp( Buff, "." ) != 0 ) {
        break; //exit
      }
      if ( Dict[ pType ].Sub != sSTRUCT ) {
        Stop( eSTRUCTEXP );
      }
      while ( nPtr1 > 0 ) {
        word P3 = Peek();
        Node[ P3 ].ID = iADDR;
        *P2 = P3;
        P2 = &Node[ P3 ].pRight;
        -- nPtr1;
        Call = 0;
      }
      if ( Call != 0 ) {
        Stop( eNOTAVAIL );
      }
      Scan( Buff );
      word I = Dict[ pType ].pIndx;
      do {
        if ( I >= Dict[ pType ].nIndx ) {
          Stop( eUNDEFINED );
        }
        if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
          break; //exit
        }
        ++ I;
      }
      while ( 1 );
      word P3 = Peek();
      Node[ P3 ].ID = iFIELD;
      Node[ P3 ].Value = I;
      pType = Parm[ I ].pType;
      nPtr1 = Parm[ I ].nPtr;
      pIndx = Parm[ I ].pIndx;
      nIndx = Parm[ I ].nIndx;
      Call = 0;
      *P2 = P3;
      P2 = &Node[ P3 ].pRight;
    }
    while ( 1 );
    if ( Prty >= pLVALUE && strcmp( Buff, "=" ) != 0 ) {
      if ( Call == 0 || nPtr > 0 ) {
        Stop( eASSIGNEXP );
      }
      nPtr1 = 0;
    }
    word P4 = nNODE;
    if ( Prty < pINC ) {
      if ( nPtr > nPtr1 + 1 ) {
        Stop( eNOADDR );
      }
      P4 = Peek();
      if ( nPtr > nPtr1 ) {
        Node[ P4 ].ID = iLPTR;
      }
      else {
        Node[ P4 ].ID = iLOAD;
      }
    }
    else {
      if ( ( nPtr > nPtr1 ) || ( Node[ P1 ].ID == iCHAR ) || ( Node[ P1 ].ID == iWORD ) || ( Node[ P1 ].ID == iSTRING ) || ( nPtr >= nPtr1 && Call != 0 && strcmp( Buff, "=" ) == 0 ) ) {
        Stop( eNOADDR );
      }
    }
    while ( nPtr1 > nPtr ) {
      word P3 = Peek();
      Node[ P3 ].ID = iADDR;
      *P2 = P3;
      P2 = &Node[ P3 ].pRight;
      -- nPtr1;
    }
    *P2 = P4;
    Flag = 1;
  }
  if ( Flag == 0 ) {
    if ( Prty >= pINC ) {
      Stop( eNOADDR );
    }
    Scan( Buff );
  }
  do {
    word ID;
    word P;
    //{//select
    if ( strcmp( Buff, "|" ) == 0 ) {
      ID = iOR;
      P = pBOOL;
    }
    else if ( strcmp( Buff, "^" ) == 0 ) {
      ID = iXOR;
      P = pBOOL;
    }
    else if ( strcmp( Buff, "&" ) == 0 ) {
      ID = iAND;
      P = pBOOL;
    }
    else if ( strcmp( Buff, "<" ) == 0 ) {
      ID = iLT;
      P = pCOMP;
    }
    else if ( strcmp( Buff, "<=" ) == 0 ) {
      ID = iLE;
      P = pCOMP;
    }
    else if ( strcmp( Buff, "=" ) == 0 ) {
      ID = iEQ;
      P = pCOMP;
    }
    else if ( strcmp( Buff, "!=" ) == 0 ) {
      ID = iNE;
      P = pCOMP;
    }
    else if ( strcmp( Buff, ">=" ) == 0 ) {
      ID = iGE;
      P = pCOMP;
    }
    else if ( strcmp( Buff, ">" ) == 0 ) {
      ID = iGT;
      P = pCOMP;
    }
    else if ( strcmp( Buff, "+" ) == 0 ) {
      ID = iADD;
      P = pADD;
    }
    else if ( strcmp( Buff, "-" ) == 0 ) {
      ID = iSUB;
      P = pADD;
    }
    else if ( strcmp( Buff, "*" ) == 0 ) {
      ID = iMUL;
      P = pMUL;
    }
    else if ( strcmp( Buff, "/" ) == 0 ) {
      ID = iDIV;
      P = pMUL;
    }
    else if ( strcmp( Buff, "%" ) == 0 ) {
      ID = iMOD;
      P = pMUL;
    }
    else { //default
      P = pZERO;
    }
    if ( P <= Prty ) {
      break; //exit
    }
    word pType2, nPtr2;
    word P2 = Peek();
    Node[ P2 ].ID = ID;
    Node[ P2 ].pLeft = P1;
    Node[ P2 ].pRight = Expr( P, pType2, nPtr2, Scan( Buff ) );
    if ( nPtr == 0 && nPtr2 == 0 ) {
      if ( pType2 != pType ) {
        //{//select
        if ( pType == Find( "byte" ) && pType2 == Find( "word" ) ) {
          Cast( pType2, pType, Node[ P2 ].pLeft );
          pType = pType2;
        }
        else if ( pType == Find( "byte" ) && pType2 == Find( "int" ) ) {
          Cast( pType2, pType, Node[ P2 ].pLeft );
          pType = pType2;
        }
        else if ( pType == Find( "word" ) && pType2 == Find( "byte" ) ) {
          Cast( pType, pType2, Node[ P2 ].pRight );
        }
        else if ( pType == Find( "word" ) && pType2 == Find( "int" ) ) {
          Cast( pType2, pType, Node[ P2 ].pLeft );
          pType = pType2;
        }
        else if ( pType == Find( "int" ) && pType2 == Find( "byte" ) ) {
          Cast( pType, pType2, Node[ P2 ].pRight );
        }
        else if ( pType == Find( "int" ) && pType2 == Find( "word" ) ) {
          Cast( pType, pType2, Node[ P2 ].pRight );
        }
        else { //default
          Stop( eTYPE );
        }
      }
    }
    else {
      if ( nPtr == 0 || Node[ Node[ P2 ].pRight ].ID != iNULL ) {
        Stop( eTYPE );
      }
    }
    //{//select
    if ( nPtr > 0 ) {
      if ( ID != iEQ && ID != iNE ) {
        Stop( eTYPE );
      }
      pType = nDICT;
    }
    else if ( ID == iOR ) {
      //{//select
      if ( pType == Find( "byte" ) || pType == Find( "word" ) ) {
        Node[ P2 ].ID = iBIT_OR;
      }
      else if ( pType < nDICT ) {
        Stop( eTYPE );
      }
    }
    else if ( ID == iXOR ) {
      //{//select
      if ( pType == Find( "byte" ) || pType == Find( "word" ) ) {
        Node[ P2 ].ID = iBIT_XOR;
      }
      else if ( pType >= nDICT ) {
        word P3 = Peek();
        Node[ P3 ]. ID = iEMPTY;
        Node[ P3 ].pLeft = Node[ P2 ].pLeft;
        Node[ P3 ].pRight = Node[ P2 ].pRight;
        Node[ P2 ].pLeft = P3;
        Node[ P2 ].pRight = Dup( Node[ P2 ].pRight );
      }
      else { //default
        Stop( eTYPE );
      }
    }
    else if ( ID == iAND ) {
      //{//select
      if ( pType == Find( "byte" ) || pType == Find( "word" ) ) {
        Node[ P2 ].ID = iBIT_AND;
      }
      else if ( pType < nDICT ) {
        Stop( eTYPE );
      }
    }
    else if ( iLT <= ID && ID <= iGT ) {
      if ( pType != Find( "char" ) && pType != Find( "byte" ) && pType != Find( "word" ) && pType != Find( "int" ) ) {
        Stop( eTYPE );
      }
      pType = nDICT;
    }
    else if ( iADD <= ID && ID <= iDIV ) {
      if ( pType != Find( "byte" ) && pType != Find( "word" ) && pType != Find( "int" ) ) {
        Stop( eTYPE );
      }
    }
    else if ( ID == iMOD ) {
      if ( pType != Find( "byte" ) && pType != Find( "word" ) ) {
        Stop( eTYPE );
      }
    }
    else { //default
      Stop( eINTERNAL );
    }
    P1 = P2;
  }
  while ( 1 );
  return P1;
}
word Ctrl( char *Buff )
{
  word P1 = Peek();
  //{//select
  if ( strcmp( Buff, "if" ) == 0 || strcmp( Buff, "select" ) == 0 ) {
    Node [ P1 ].ID = iSELECT;
    char *Case = "";
    char *Then = "then";
    char *Else = "else";
    if ( strcmp( Buff, "select" ) == 0 ) {
      Case = "case";
      Then = ":";
      Else = "default";
      if ( strcmp( Scan( Buff ), "case" ) != 0 ) {
        Stop( eCASEEXP );
      }
    }
    else {
      strcpy( Buff, Case );
    }
    word *P2 = &Node[ P1 ].pLeft;
    while ( strcmp( Buff, "end" ) != 0 ) {
      word P3 = Peek();
      word P4 = Peek();
      Node [ P3 ]. ID = iEMPTY;
      Node [ P3 ].pLeft = P4;
      Node [ P4 ]. ID = iEMPTY;
      if ( strcmp( Buff, Case ) == 0 ) {
        word pType, nPtr;
        Node [ P4 ].pLeft = Expr( pZERO, pType, nPtr, Scan( Buff ) );
        if ( strcmp( Buff, Then ) != 0 ) {
          Stop( eTHENEXP );
        }
        if ( pType < nDICT ) {
          Stop( eTYPE );
        }
      }
      else {
        if ( strcmp( Else, "default" ) == 0 ) {
          if ( strcmp( Scan( Buff ), ":" ) != 0 ) {
            Stop( eCOLONEXP );
          }
        }
        Case = "";
        Else = "";
      }
      word *P5 = &Node[ P4 ].pRight;
      word nParm1 = nParm;
      word nIndx1 = nIndx;
      word Flag = 0;
      Scan( Buff );
      while ( Flag == 0 || ( strcmp( Buff, Case ) != 0 && strcmp( Buff, Else ) != 0 && strcmp( Buff, "end" ) != 0 ) ) {
        *P5 = Ctrl( Buff );
        P5 = &Node[ *P5 ].pRight;
        Flag = 1;
      }
      nIndx = nIndx1;
      nParm = nParm1;
      *P2 = P3;
      P2 = &Node[ P3 ].pRight;
    }
  }
  else if ( strcmp( Buff, "while" ) == 0 || strcmp( Buff, "do" ) == 0 ) {
    word P2 = Peek();
    word P3 = Peek();
    Node [ P1 ].ID = iWHILE;
    Node [ P1 ].pLeft = P2;
    Node [ P2 ].ID = iEMPTY;
    Node [ P2 ].pLeft = P3;
    Node [ P3 ].ID = iEMPTY;
    if ( strcmp( Buff, "while" ) == 0 ) {
      word pType, nPtr;
      Node[ P3 ].pLeft = Expr( pZERO, pType, nPtr, Scan( Buff ) );
      if ( strcmp( Buff, "do" ) != 0 ) {
        Stop( eDOEXP );
      }
      if ( pType < nDICT ) {
        Stop( eTYPE );
      }
    }
    word *P4 = &Node[ P2 ].pRight;
    word nParm1 = nParm;
    word nIndx1 = nIndx;
    word pLoop1 = pLoop;
    word Flag = 0;
    pLoop = P1;
    Scan( Buff );
    while ( Flag == 0 || strcmp( Buff, "end" ) != 0 ) {
      *P4 = Ctrl( Buff );
      P4 = &Node[ *P4 ].pRight;
      Flag = 1;
    }
    pLoop = pLoop1;
    nIndx = nIndx1;
    nParm = nParm1;
  }
  else if ( strcmp( Buff, "loop" ) == 0 ) {
    if ( pLoop >= nNODE ) {
      Stop( eWHILEEXP );
    }
    Node[ P1 ].ID = iLOOP;
    Node[ P1 ].Value = Node[ Node[ pLoop ].pLeft ].pLeft;
  }
  else if ( strcmp( Buff, "exit" ) == 0 ) {
    if ( pLoop >= nNODE ) {
      Stop( eWHILEEXP );
    }
    Node[ P1 ].ID = iEXIT;
    Node[ P1 ].Value = pLoop;
  }
  else if ( strcmp( Buff, "inc" ) == 0 || strcmp( Buff, "dec" ) == 0 ) {
    if ( strcmp( Buff, "inc" ) == 0 ) {
      Node[ P1 ].ID = iINC;
    }
    else {
      Node[ P1 ].ID = iDEC;
    }
    word pType, nPtr;
    Node[ P1 ].pLeft = Expr( pINC, pType, nPtr, Scan( Buff ) );
    if ( strcmp( Buff, ";" ) != 0 ) {
      Stop( eSEMICOLONEXP );
    }
    if ( ( pType != Find( "byte" ) && pType != Find( "word" ) && pType != Find( "int" ) ) || nPtr != 0 ) {
      Stop( eTYPE );
    }
  }
  else if ( strcmp( Buff, "return" ) == 0 ) {
    Node[ P1 ].ID = iRETURN;
    if ( Dict[ pFunc ].nPtr != 0 || strcmp( Dict[ Dict[ pFunc ].pType ].Name, "void" ) != 0 ) {
      word pType2, nPtr2;
      Node[ P1 ].pLeft = Expr( pZERO, pType2, nPtr2, Scan( Buff ) );
      if ( strcmp( Buff, ";" ) != 0 ) {
        Stop( eSEMICOLONEXP );
      }
      Test( Dict[ pFunc ].pType, Dict[ pFunc ].nPtr, pType2, nPtr2, Node[ P1 ].pLeft );
    }
  }
  else if ( strcmp( Buff, "null" ) == 0 ) {
    Node[ P1 ].ID = iEMPTY;
  }
  else { //default
    word pType1, nPtr1;
    word P2 = Expr( pLVALUE, pType1, nPtr1, Buff );
    if ( P2 < nNODE ) {
      word P3 = Peek();
      Node[ P1 ].ID = iASSIGN;
      Node[ P1 ].pLeft = P3;
      Node[ P3 ].ID = iEMPTY;
      Node[ P3 ].pLeft = P2;
      if ( strcmp( Buff, "=" ) == 0 ) {
        word pType2, nPtr2;
        Node[ P3 ].pRight = Expr( pZERO, pType2, nPtr2, Scan( Buff ) );
        Test( pType1, nPtr1, pType2, nPtr2, Node[ P3 ].pRight );
      }
    }
    else {
      word pType = Find( Buff );
      if ( pType >= nDict || Dict[ pType ].Class != cTYPE ) {
        Stop( eUNDEFINED );
      }
      word nPtr = 0;
      while ( strcmp( Scan( Buff ), "@" ) == 0 ) {
        ++ nPtr;
      }
      if ( pType == Find( "void" ) && nPtr < 1 ) {
        Stop( eNOVOID );
      }
      word I = pParm;
      while ( I < nParm ) {
        if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
          Stop( eDUPLICATE );
        }
        ++ I;
      }
      if ( nParm >= nPARM ) {
        Stop( eNOMEMORY );
      }
      Parm[ nParm ].pType = pType; Copy( Parm[ nParm ].Name, Buff, 0 );
      Parm[ nParm ].nPtr = nPtr;
      Parm[ nParm ].pIndx = nIndx;
      Parm[ nParm ].nIndx = Tabl( Scan( Buff ) );
      word N = 1;
      word K = Parm[ nParm ].pIndx;
      while ( K < Parm[ nParm ].nIndx ) {
        N = Indx[ K ] * N;
        ++ K;
      }
      word P3 = Peek();
      Node[ P1 ].ID = iDEF;
      Node[ P1 ].Value = pType;
      Node[ P1 ].pLeft = P3;
      word P4 = Peek();
      Node[ P3 ].ID = iEMPTY;
      Node[ P3 ].Value = nPtr;
      Node[ P3 ].pLeft = P4;
      Node[ P4 ].ID = iEMPTY;
      Node[ P4 ].Value = N;
      if ( strcmp( Buff, "=" ) == 0 ) {
        if ( Parm[ nParm ].pIndx < Parm[ nParm ].nIndx ) {
          Stop( eNOTAVAIL );
        }
        word P5 = Peek();
        Node[ P4 ].ID = iASSIGN;
        Node[ P4 ].pLeft = P5;
        word pType2, nPtr2;
        word P6 = Peek();
        Node[ P5 ].ID = iEMPTY;
        Node[ P5 ].pLeft = P6;
        Node[ P5 ].pRight = Expr( pZERO, pType2, nPtr2, Scan( Buff ) );
        Node[ P6 ].ID = iLOCAL;
        Node[ P6 ].Value = nParm - pParm;
        Test( pType, nPtr, pType2, nPtr2, Node[ P5 ].pRight );
      }
      ++ nParm;
      if ( strcmp( Buff, "," ) == 0 ) {
        strcpy( Buff, Dict[ pType ].Name );
        return P1;
      }
    }
    if ( strcmp( Buff, ";" ) != 0 ) {
      Stop( eSEMICOLONEXP );
    }
  }
  Scan ( Buff );
  return P1;
}
void Save( char Ch )
{
  if ( nText >= nTEXT ) {
    write( hText, Text, nText );
    nText = 0;
  }
  Text[ nText ] = Ch;
  ++ nText;
}
void Decl( char *Inst )
{
  word I = 0;
  while ( Inst[ I ] != 0 ) {
    Save( Inst[ I ] );
    ++ I;
  }
//  Save( '\r' );
  Save( '\n' );
}
void Emit( word L, char *Inst )
{
  if ( L != 0 ) {
    Save( '@' );
    char *P = str( L );
    word I = 0;
    while ( P[ I ] != 0 ) {
      Save( P[ I ] );
      ++ I;
    }
    Save( ':' );
    Save( ' ' );
  }
  else {
    word I = 0;
    while ( I < 8 ) {
      Save( ' ' );
      ++ I;
    }
  }
  Decl( Inst );
}
word Even( word Ofs )
{
  while ( Ofs % 4 != 0 ) {
    ++ Ofs;
  }
  return Ofs;
}
void Mark( word P )
{
  if ( P >= nNODE ) {
    return ;
  }
  //{//select
  if ( Node[ P ].ID == iCALL ) {
    if ( Dict[ Node[ P ].Value ].Flag == 0 ) {
      Dict[ Node[ P ].Value ].Flag = 1;
      if ( Dict[ Node[ P ].Value ].Sub == sFUNC ) {
        Mark( Dict[ Node[ P ].Value ].pNode );
      }
    }
  }
  else if ( Node[ P ].ID == iDATA ) {
    //{//select
    if ( Node[ P ].pLeft >= nNODE ) {
      Dict[ Node[ P ].Value ].Flag = 1;
    }
    else if ( Node[ Node[ P ].pLeft ].ID != iCALL ) {
      Dict[ Node[ P ].Value ].Flag = 1;
    }
  }
  else if ( Node[ P ].ID == iFUNC ) {
    if ( Dict[ Node[ P ].Value ].Flag == 0 ) {
      Dict[ Node[ P ].Value ].Flag = 1;
      if ( Dict[ Node[ P ].Value ].Sub == sFUNC ) {
        Mark( Dict[ Node[ P ].Value ].pNode );
      }
    }
  }
  else if ( Node[ P ].ID == iSTRING ) {
    Dict[ Node[ P ].Value ].Flag = 1;
  }
  Mark( Node[ P ].pLeft );
  Mark( Node[ P ].pRight );
}
word Enum( word P, word Flag, word &L )
{
  if ( P >= nNODE ) {
    return 0;
  }
  //{//select
  if ( Flag != 0 ) {
    word S = 0;
    word M = 0;
    while ( P < nNODE ) {
      //{//select
      if ( Node[ P ].ID == iSELECT ) {
        word P1 = Node[ P ].pLeft;
        while ( P1 < nNODE ) {
          word P2 = Node[ P1 ].pLeft;
          Enum( Node[ P2 ].pLeft, 0, L );
          Node[ P2 ].Value = L;
          ++ L;
          word V = Enum( Node[ P2 ].pRight, 1, L );
          if ( M < S + V ) {
            M = S + V;
          }
          Node[ P1 ].Value = L;
          ++ L;
          P1 = Node[ P1 ].pRight;
        }
        Node[ P ].Value = L;
        ++ L;
      }
      else if ( Node[ P ].ID == iWHILE ) {
        word P1 = Node[ P ] .pLeft;
        word P2 = Node[ P1 ].pLeft;
        Node[ P2 ].Value = L;
        ++ L;
        Enum( Node[ P2 ].pLeft, 0, L );
        Node[ P1 ].Value = L;
        ++ L;
        word V = Enum( Node[ P1 ].pRight, 1, L );
        if ( M < S + V ) {
          M = S + V;
        }
        Node[ P ].Value = L;
        ++ L;
      }
      else if ( Node[ P ].ID == iDEF ) {
        word Size = 4;
        if ( Node[ Node[ P ].pLeft ].Value == 0 ) {
          Size = Dict[ Node[ P ].Value ].Value;
        }
        S = S + Even( Node[ Node[ Node[ P ].pLeft ].pLeft ].Value * Size );
        if ( M < S ) {
          M = S;
        }
      }
      P = Node[ P ].pRight;
    }
    return M;
  }
  else if ( Node[ P ].ID != iASSIGN && Node[ P ].ID != iINC && Node[ P ].ID != iDEC ) {
    Enum( Node[ P ].pLeft, 0, L );
    if ( ( iOR <= Node[ P ].ID && Node[ P ].ID <= iAND ) || ( Node[ P ].ID == iEMPTY ) ) {
      Node[ P ].Value = L;
      ++ L;
    }
    Enum( Node[ P ].pRight, 0, L );
  }
  return 0;
}
word pSize;
word pVar;
word fEAX;
word Code( word P, word F, word T, word M )
{
  char Buff [ nBUFF ];
  //{//select
  if ( Node[ P ].ID == iNULL ) {
    if ( fEAX == 0 ) {
      Emit( 0, "xor     EAX,  EAX" );
    }
    else {
      Emit( 0, "xor     EBX,  EBX" );
    }
    return nDICT;
  }
  else if ( Node[ P ].ID == iCHAR ) {
    if ( fEAX == 0 ) {
      Emit( 0, strcat( strcpy( Buff, "mov     AL,   " ), str( Node[ P ].Value ) ) );
    }
    else {
      Emit( 0, strcat( strcpy( Buff, "mov     BL,   " ), str( Node[ P ].Value ) ) );
    }
    return Find( "char" );
  }
  else if ( Node[ P ].ID == iWORD ) {
    if ( fEAX == 0 ) {
      Emit( 0, strcat( strcpy( Buff, "mov     EAX,  " ), str( Node[ P ].Value ) ) );
    }
    else {
      Emit( 0, strcat( strcpy( Buff, "mov     EBX,  " ), str( Node[ P ].Value ) ) );
    }
    return Find( "word" );
  }
  else if ( Node[ P ].ID == iINT ) {
    if ( fEAX == 0 ) {
      Emit( 0, strcat( strcpy( Buff, "mov     EAX,  " ), str( Node[ P ].Value ) ) );
    }
    else {
      Emit( 0, strcat( strcpy( Buff, "mov     EBX,  " ), str( Node[ P ].Value ) ) );
    }
    return Find( "int" );
  }
  else if ( Node[ P ].ID == iFUNC ) {
    if ( fEAX == 0 ) {
      Emit( 0, strcat( strcpy( Buff, "mov     EAX,  @" ), str( Dict[ Node[ P ].Value ].Value ) ) );
    }
    else {
      Emit( 0, strcat( strcpy( Buff, "mov     EBX,  @" ), str( Dict[ Node[ P ].Value ].Value ) ) );
    }
    return nDICT;
  }
  else if ( Node[ P ].ID == iDATA || Node[ P ].ID == iPARM || Node[ P ].ID == iLOCAL || Node[ P ].ID == iSTRING || Node[ P ].ID == iINC || Node[ P ].ID == iDEC ) {
    word ID1 = Node[ P ].ID;
    if ( ID1 == iINC || ID1 == iDEC ) {
      fEAX = 0;
      P = Node[ P ].pLeft;
    }
    word pType;
    word nPtr;
    word Ofs;
    //{//select
    if ( Node[ P ].ID == iDATA ) {
      pType = Dict[ Node[ P ].Value ].pType;
      nPtr = Dict[ Node[ P ].Value ].nPtr;
      Ofs = Dict[ Node[ P ].Value ]. Value;
    }
    else if ( Node[ P ].ID == iPARM ) {
      pType = Parm[ Node[ P ].Value ].pType;
      nPtr = Parm[ Node[ P ].Value ].nPtr;
      Ofs = Parm[ Node[ P ].Value ]. Value;
    }
    else if ( Node[ P ].ID == iLOCAL ) {
      pType = Parm[ Node[ P ].Value + pParm ].pType;
      nPtr = Parm[ Node[ P ].Value + pParm ].nPtr;
      Ofs = Parm[ Node[ P ].Value + pParm ]. Value;
    }
    else if ( Node[ P ].ID == iSTRING ) {
      pType = Dict[ Node[ P ].Value ].pType;
      nPtr = 1;
      Ofs = Dict[ Node[ P ].Value ]. Value;
    }
    else { //default
      Stop( eINTERNAL );
    }
    word ID = Node[ P ]. ID;
    word IX = 0;
    word P1 = Node[ P ].pLeft;
    do {
      //{//select
      if ( P1 >= nNODE ) {
        //{//select
        if ( ID1 == iINC || ID1 == iDEC ) {
          if ( IX == 2 || nPtr > 0 ) {
            Stop( eINTERNAL );
          }
          if ( ID1 == iINC ) {
            strcpy( Buff, "inc     " );
          }
          else {
            strcpy( Buff, "dec     " );
          }
          word Size = Dict[ pType ].Value;
          //{//select
          if ( Size == 1 ) {
            strcat( Buff, "byte " );
          }
          else if ( Size == 4 ) {
            strcat( Buff, "dword " );
          }
          else { //default
            Stop( eINTERNAL );
          }
          if ( IX != 0 ) {
            //{//select
            if ( ID == iDATA ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[@@DATA+EAX+" ), str( Dict[ Node[ P ].Value ].Value ) ), "]" ) );
            }
            else if ( ID == iPARM ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[EBP+EAX+" ), str( Parm[ Node[ P ].Value ].Value ) ), "]" ) ); // 03.01.2005
            }
            else if ( ID == iLOCAL ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[EBP+EAX-" ), str( Parm[ Node[ P ].Value + pParm ].Value ) ), "]" ) ); // 03.01.2005
            }
            else if ( ID == iREF ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[EAX+" ), str( Ofs ) ), "]" ) );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
          else {
            //{//select
            if ( ID == iDATA ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[@@DATA+" ), str( Dict[ Node[ P ].Value ].Value ) ), "]" ) );
            }
            else if ( ID == iPARM ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[EBP+" ), str( Parm[ Node[ P ].Value ].Value ) ), "]" ) );
            }
            else if ( ID == iLOCAL ) {
              Emit( 0, strcat( strcat( strcat( Buff, "[EBP-" ), str( Parm[ Node[ P ].Value + pParm ].Value ) ), "]" ) );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
        }
        else if ( ID != iCALL ) {
          word Size = 4;
          if ( nPtr == 0 ) {
            Size = Dict[ pType ].Value;
          }
          //{//select
          if ( Size == 1 ) {
            strcpy( Buff, "mov     byte  " ); // 03.01.2005
          }
          else if ( Size == 4 ) {
            strcpy( Buff, "mov     dword " );
          }
          else { //default
            Stop( eINTERNAL );
          }
          if ( IX != 0 ) {
            char RX[ 4 ];
            if ( IX == 1 ) {
              strcpy( RX, "EAX" );
            }
            else {
              strcpy( RX, "EBX" );
            }
            //{//select
            if ( ID == iDATA ) {
              strcat( strcat( strcat( Buff, "[@@DATA+" ), RX ), "+" ); // @@DATA // 03.01.2005
            }
            else if ( ID == iPARM ) {
              strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "+" ); // 03.01.2005
            }
            else if ( ID == iLOCAL ) {
              strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "-" ); // 03.01.2005
            }
            else if ( ID == iREF ) {
              strcat( strcat( strcat( Buff, "[" ), RX ), "+" );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
          else {
            //{//select
            if ( ID == iDATA ) {
              strcat( Buff, "[@@DATA+" );
            }
            else if ( ID == iPARM ) {
              strcat( Buff, "[EBP+" );
            }
            else if ( ID == iLOCAL ) {
              strcat( Buff, "[EBP-" );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
          strcat( strcat( Buff, str( Ofs ) ), "], " );
          //{//select
          if ( fEAX != 0 ) {
            //{//select
            if ( Size == 1 ) {
              Emit( 0, strcat( Buff, "AL" ) );
            }
            else if ( Size == 4 ) {
              Emit( 0, strcat( Buff, "EAX" ) );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
          else if ( IX == 1 ) {
            Emit( 0, "pop     EBX" );
            //{//select
            if ( Size == 1 ) {
              Emit( 0, strcat( Buff, "BL" ) );
            }
            else if ( Size == 4 ) {
              Emit( 0, strcat( Buff, "EBX" ) );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
          else if ( IX == 2 ) {
            Emit( 0, "pop     EAX" );
            //{//select
            if ( Size == 1 ) {
              Emit( 0, strcat( Buff, "AL" ) );
            }
            else if ( Size == 4 ) {
              Emit( 0, strcat( Buff, "EAX" ) );
            }
            else { //default
              Stop( eINTERNAL );
            }
          }
        }
        break; //exit
      }
      else if ( Node[ P1 ].ID == iLOAD ) {
        word Size = 4;
        if ( nPtr == 0 ) {
          Size = Dict[ pType ].Value;
        }
        //{//select
        if ( Size == 1 ) {
          if ( fEAX == 0 ) {
            strcpy( Buff, "mov     AL,   byte  " ); // 03.01.2005
          }
          else {
            strcpy( Buff, "mov     BL,   byte  " ); // 03.01.2005
          }
        }
        else if ( Size == 4 ) {
          if ( fEAX == 0 ) {
            strcpy( Buff, "mov     EAX,  dword " );
          }
          else {
            strcpy( Buff, "mov     EBX,  dword " );
          }
        }
        else { //default
          Stop( eINTERNAL );
        }
        if ( IX != 0 ) {
          char RX[ 4 ];
          if ( IX == 1 ) {
            strcpy( RX, "EAX" );
          }
          else {
            strcpy( RX, "EBX" );
          }
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[@@DATA+" ), RX ), "+" ), str( Ofs ) ), "]" ) ); // @@DATA // 03.01.2005
          }
          else if ( ID == iPARM ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "+" ), str( Ofs ) ), "]" ) ); // 03.01.2005
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "-" ), str( Ofs ) ), "]" ) ); // 03.01.2005
          }
          else if ( ID == iREF ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[" ), RX ), "+" ), str( Ofs ) ), "]" ) );
          }
          else { //default
            Stop( eINTERNAL );
          }
        }
        else {
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[@@DATA+" ), str( Ofs ) ), "]" ) );
          }
          else if ( ID == iPARM ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[EBP+" ), str( Ofs ) ), "]" ) );
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[EBP-" ), str( Ofs ) ), "]" ) );
          }
          else if ( ID != iCALL ) {
            Stop( eINTERNAL );
          }
        }
        if ( nPtr > 0 ) {
          return nDICT;
        }
        else {
          return pType;
        }
      }
      else if ( Node[ P1 ].ID == iLPTR ) {
        if ( fEAX != 0 || IX == 2 ) {
          Stop( eINTERNAL );
        }
        if ( IX != 0 ) {
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcpy( Buff, "add     EAX,  @@DATA+" ), str( Ofs ) ), "" ) );
          }
          else if ( ID == iPARM ) {
            Emit( 0, "add     EAX,  EBP" );
            Emit( 0, strcat( strcpy( Buff, "add     EAX,  " ), str( Ofs ) ) );
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, "add     EAX,  EBP" );
            Emit( 0, strcat( strcpy( Buff, "sub     EAX,  " ), str( Ofs ) ) );
          }
          else if ( ID == iSTRING ) {
            Emit( 0, strcat( strcat( strcpy( Buff, "add     EAX,  @@TEXT+" ), str( Ofs ) ), "" ) );
          }
          else if ( ID == iREF && Ofs != 0 ) {
            Emit( 0, strcat( strcpy( Buff, "add     EAX,  " ), str( Ofs ) ) );
          }
          else if ( ID != iREF && ID != iCALL ) {
            Stop( eINTERNAL );
          }
        }
        else {
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcpy( Buff, "mov     EAX,  @@DATA+" ), str( Ofs ) ), "" ) );
          }
          else if ( ID == iPARM ) {
            Emit( 0, "mov     EAX,  EBP" );
            Emit( 0, strcat( strcpy( Buff, "add     EAX,  " ), str( Ofs ) ) );
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, "mov     EAX,  EBP" );
            Emit( 0, strcat( strcpy( Buff, "sub     EAX,  " ), str( Ofs ) ) );
          }
          else if ( ID == iSTRING ) {
            Emit( 0, strcat( strcat( strcpy( Buff, "mov     EAX,  @@TEXT+" ), str( Ofs ) ), "" ) );
          }
          else if ( ID != iCALL ) {
            Stop( eINTERNAL );
          }
        }
        return nDICT;
      }
      else if ( Node[ P1 ].ID == iCALL ) {
        if ( fEAX != 0 ) {
          Emit( 0, "push    EAX" );
          fEAX = 0;
        }
        if ( Node[ P1 ].pLeft < nNODE ) {
          Code( Node[ P1 ].pLeft, 0, 0, 0 );
        }
        if ( Dict[ Node[ P1 ].Value ].Sub == sSYS ) {
          Emit( 0, strcat( strcat( strcpy( Buff, "call    [" ), Dict[ Node[ P1 ].Value ].Name ), "]" ) );
        }
        else {
          Emit( 0, strcat( strcpy( Buff, "call    @" ), str( Dict[ Node[ P1 ].Value ].Value ) ) );
        }
        ID = iCALL;
        Ofs = 0;
      }
      else if ( Node[ P1 ].ID == iINDEX ) {
        word Indx = 0;
        if ( Node[ P1 ].pRight < nNODE ) {
          if ( Node[ Node[ P1 ].pRight ].ID == iINDEX ) {
            if ( fEAX != 0 ) {
              Emit( 0, "push    EAX" );
              fEAX = 0;
            }
            //{//select
            if ( IX == 1 ) {
              Emit( 0, "push    EAX" );
              Indx = 1;
            }
            else if ( IX == 2 ) {
              Emit( 0, "push    EBX" );
              Indx = 1;
            }
            IX = 0;
          }
        }
        word Size = 4;
        if ( nPtr == 0 ) {
          Size = Dict[ pType ].Value;
        }
        word Flag = 0;
        do {
          if ( Flag == 0 ) {
            if ( IX != 0 ) {
              if ( IX == 2 ) {
                if ( fEAX != 0 ) {
                  Emit( 0, "push     EAX" );
                }
                Emit( 0, "xchg    EAX,  EBX" );
                IX = 1;
              }
              fEAX = 1;
              Code( Node[ P1 ].pLeft, 0, 0, 0 );
              if ( fEAX == 0 ) {
                Indx = 1;
              }
              else {
                Indx = 2;
              }
              fEAX = 0;
            }
            else {
              Code( Node[ P1 ].pLeft, 0, 0, 0 );
              if ( fEAX == 0 ) {
                IX = 1;
              }
              else {
                IX = 2;
              }
            }
          }
          else {
            Emit( 0, strcat( strcpy( Buff, "imul    EAX,  " ), str( Node[ P1 ].Value ) ) );
            fEAX = 1;
            Code( Node[ P1 ].pLeft, 0, 0, 0 );
            if ( fEAX == 0 ) {
              Emit( 0, "pop     EBX" );
            }
            Emit( 0, "add     EAX,  EBX" );
            fEAX = 0;
          }
          if ( Node[ P1 ].pRight >= nNODE ) {
            break; //exit
          }
          if ( Node[ Node[ P1 ].pRight ].ID != iINDEX ) {
            break; //exit
          }
          P1 = Node[ P1 ].pRight;
          Flag = 1;
        }
        while ( 1 );
        //{//select
        if ( Size == 4 ) {
          if ( IX == 1 ) {
            Emit( 0, "shl     EAX,  2" );
          }
          else {
            Emit( 0, "shl     EBX,  2" );
          }
        }
        else if ( Size > 1 ) {
          if ( IX == 1 ) {
            Emit( 0, strcat( strcpy( Buff, "imul    EAX,  " ), str( Size ) ) );
          }
          else {
            Emit( 0, strcat( strcpy( Buff, "imul    EBX,  " ), str( Size ) ) );
          }
        }
        if ( Indx == 1 ) {
          Emit( 0, "pop     EBX" );
        }
        if ( Indx != 0 ) {
          Emit( 0, "add     EAX,  EBX" );
        }
      }
      else if ( Node[ P1 ].ID == iADDR ) {
        if ( nPtr < 1 ) {
          Stop( eINTERNAL );
        }
        -- nPtr;
        if ( Node[ P1 ].pLeft < nNODE ) {
          word Size = 4;
          if ( nPtr == 0 ) {
            Size = Dict[ pType ].Value;
          }
          //{//select
          if ( IX == 2 ) {
            if ( fEAX != 0 ) {
              Emit( 0, "push    EAX" );
            }
            Emit( 0, "xchg    EAX,  EBX" );
            IX = 1;
            fEAX = 1;
          }
          else if ( IX == 1 ) {
            fEAX = 1;
          }
          Code( Node[ P1 ].pLeft, 0, 0, 0 );
          if ( fEAX == 0 ) {
            //{//select
            if ( Size == 4 ) {
              Emit( 0, "shl     EAX,  2" );
            }
            else if ( Size > 1 ) {
              Emit( 0, strcat( strcpy( Buff, "imul    EAX,  " ), str( Size ) ) );
            }
            if ( IX != 0 ) {
              Emit( 0, "pop     EBX" );
              IX = 2;
            }
            strcpy( Buff, "add     EAX,  dword " );
          }
          else {
            //{//select
            if ( Size == 4 ) {
              Emit( 0, "shl     EBX,  2" );
            }
            else if ( Size > 1 ) {
              Emit( 0, strcat( strcpy( Buff, "imul    EBX,  " ), str( Size ) ) );
            }
            strcpy( Buff, "add     EBX,  dword " );
          }
        }
        else {
          if ( fEAX == 0 ) {
            strcpy( Buff, "mov     EAX,  dword " );
          }
          else {
            strcpy( Buff, "mov     EBX,  dword " );
          }
        }
        if ( IX != 0 ) {
          char RX[ 4 ];
          if ( IX == 1 ) {
            strcpy( RX, "EAX" );
          }
          else {
            strcpy( RX, "EBX" );
          }
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[@@DATA+" ), RX ), "+" ), str( Ofs ) ), "]" ) ); // @@DATA // 03.01.2005
          }
          else if ( ID == iPARM ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "+" ), str( Ofs ) ), "]" ) ); // 03.01.2005
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[EBP+" ), RX ), "-" ), str( Ofs ) ), "]" ) ); // 03.01.2005
          }
          else if ( ID == iREF ) {
            Emit( 0, strcat( strcat( strcat( strcat( strcat( Buff, "[" ), RX ), "+" ), str( Ofs ) ), "]" ) );
          }
          else { //default
            Stop( eINTERNAL );
          }
        }
        else {
          //{//select
          if ( ID == iDATA ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[@@DATA+" ), str( Ofs ) ), "]" ) );
          }
          else if ( ID == iPARM ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[EBP+" ), str( Ofs ) ), "]" ) );
          }
          else if ( ID == iLOCAL ) {
            Emit( 0, strcat( strcat( strcat( Buff, "[EBP-" ), str( Ofs ) ), "]" ) );
          }
          else { //default
            Stop( eINTERNAL );
          }
        }
        if ( fEAX == 0 ) {
          IX = 1;
        }
        else {
          if ( IX != 0 ) {
            fEAX = 0;
          }
          IX = 2;
        }
        ID = iREF;
        Ofs = 0;
      }
      else if ( Node[ P1 ].ID == iFIELD ) {
        pType = Parm[ Node[ P1 ].Value ].pType;
        nPtr = Parm[ Node[ P1 ].Value ].nPtr;
        if ( ID != iLOCAL ) {
          Ofs = Ofs + Parm[ Node[ P1 ].Value ].Value;
        }
        else {
          Ofs = Ofs - Parm[ Node[ P1 ].Value ].Value;
        }
      }
      else { //default
        Stop( eINTERNAL );
      }
      P1 = Node[ P1 ].pRight;
    }
    while ( 1 );
  }
  else if ( Node[ P ].ID == iCAST ) {
    word pType = Code( Node[ Node[ P ].pLeft ].pLeft, 0, 0, 0 );
    //{//select
    if ( Node[ P ].Value == Find( "char" ) && pType == Find( "byte" ) ) {
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "char" ) && pType == Find( "word" ) ) {
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "char" ) && pType == Find( "int" ) ) {
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "byte" ) && pType == Find( "char" ) ) {
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "word" ) && pType == Find( "char" ) ) {
      if ( fEAX == 0 ) {
        Emit( 0, "and     EAX,  0FFH" );
      }
      else {
        Emit( 0, "and     EBX,  0FFH" );
      }
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "word" ) && pType == Find( "char" ) ) {
      if ( fEAX == 0 ) {
        Emit( 0, "and     EAX,  0FFH" );
      }
      else {
        Emit( 0, "and     EBX,  0FFH" );
      }
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "word" ) && pType == Find( "byte" ) ) {
      if ( fEAX == 0 ) {
        Emit( 0, "and     EAX,  0FFH" );
      }
      else {
        Emit( 0, "and     EBX,  0FFH" );
      }
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "int" ) && pType == Find( "char" ) ) {
      if ( fEAX == 0 ) {
        Emit( 0, "and     EAX,  0FFH" );
      }
      else {
        Emit( 0, "and     EBX,  0FFH" );
      }
      return Node[ P ].Value;
    }
    else if ( Node[ P ].Value == Find( "int" ) && pType == Find( "byte" ) ) {
      return Node[ P ].Value;
    }
    else { //default
      Stop( eINTERNAL );
    }
  }
  else if ( Node[ P ].ID == iNOT ) {
    word pType = Code( Node[ P ].pLeft, 0, 0, 0 );
    word Size = Dict[ pType ].Value;
    //{//select
    if ( Size == 1 ) {
      if ( fEAX == 0 ) {
        Emit( 0, "not     AL" );
      }
      else {
        Emit( 0, "not     BL" );
      }
    }
    else if ( Size == 4 ) {
      if ( fEAX == 0 ) {
        Emit( 0, "not     EAX" );
      }
      else {
        Emit( 0, "not     EBX" );
      }
    }
    else { //default
      Stop( eINTERNAL );
    }
    return pType;
  }
  else if ( Node[ P ].ID == iNEG ) {
    if ( Node[ Node[ P ].pLeft ].ID == iWORD ) {
      if ( fEAX == 0 ) {
        Emit( 0, strcat( strcpy( Buff, "mov     EAX,  " ), str( 0xFFFFFFFF - Node[ Node[ P ].pLeft ].Value + 1 ) ) );
      }
      else {
        Emit( 0, strcat( strcpy( Buff, "mov     EBX,  " ), str( 0xFFFFFFFF - Node[ Node[ P ].pLeft ].Value + 1 ) ) );
      }
      return Find( "int" );
    }
    Code( Node[ P ].pLeft, 0, 0, 0 );
    if ( fEAX == 0 ) {
      Emit( 0, "neg     EAX" );
    }
    else {
      Emit( 0, "neg     EBX" );
    }
    return Find( "int" );
  }
  else if ( Node[ P ].ID == iOR ) {
    if ( T == 0 ) {
      Code( Node[ P ].pLeft, 0, M, Node[ P ].Value );
      Emit( Node[ P ].Value, "" );
      Code( Node[ P ].pRight, F, 0, M );
    }
    else {
      Code( Node[ P ].pLeft, 0, T, Node[ P ].Value );
      Emit( Node[ P ].Value, "" );
      Code( Node[ P ].pRight, 0, T, M );
    }
  }
  else if ( Node[ P ].ID == iXOR ) {
    word L1 = Node[ Node[ P ].pLeft ] .Value;
    word L2 = Node[ P ].Value;
    if ( T == 0 ) {
      Code( Node[ Node[ P ].pLeft ] .pLeft, L2, 0, L1 );
      Emit( L1, "" );
      Code( Node[ Node[ P ].pLeft ] .pRight, M, 0, F );
      Emit( 0 , strcat( strcpy( Buff, "jmp     @" ), str( F ) ) );
      Emit( L2, "" );
      Code( Node[ P ].pRight, F, 0, M );
    }
    else {
      Code( Node[ Node[ P ].pLeft ] .pLeft, L2, 0, L1 );
      Emit( L1, "" );
      Code( Node[ Node[ P ].pLeft ] .pRight, T, 0, M );
      Emit( 0 , strcat( strcpy( Buff, "jmp     @" ), str( M ) ) );
      Emit( L2, "" );
      Code( Node[ P ].pRight, M, 0, T );
    }
  }
  else if ( Node[ P ].ID == iEQV ) {
    word L1 = Node[ Node[ P ].pLeft ] .Value;
    word L2 = Node[ P ].Value;
    if ( T == 0 ) {
      Code( Node[ Node[ P ].pLeft ] .pLeft, L2, 0, L1 );
      Emit( L1, "" );
      Code( Node[ Node[ P ].pLeft ] .pRight, F, 0, M );
      Emit( 0 , strcat( strcpy( Buff, "jmp     @" ), str( M ) ) );
      Emit( L2, "" );
      Code( Node[ P ].pRight, 0, F, M );
    }
    else {
      Code( Node[ Node[ P ].pLeft ] .pLeft, L2, 0, L1 );
      Emit( L1, "" );
      Code( Node[ Node[ P ].pLeft ] .pRight, 0, T, M );
      Emit( 0 , strcat( strcpy( Buff, "jmp     @" ), str( M ) ) );
      Emit( L2, "" );
      Code( Node[ P ].pRight, T, 0, M );
    }
  }
  else if ( Node[ P ].ID == iAND ) {
    if ( T == 0 ) {
      Code( Node[ P ].pLeft, F, 0, Node[ P ].Value );
      Emit( Node[ P ].Value, "" );
      Code( Node[ P ].pRight, F, 0, M );
    }
    else {
      Code( Node[ P ].pLeft, M, 0, Node[ P ].Value );
      Emit( Node[ P ].Value, "" );
      Code( Node[ P ].pRight, 0, T, M );
    }
  }
  else if ( iBIT_OR <= Node[ P ].ID && Node[ P ].ID <= iMOD ) {
    if ( iLT <= Node[ P ].ID && Node[ P ].ID <= iGT ) {
      fEAX = 0;
    }
    if ( fEAX != 0 ) {
      Emit( 0, "push    EAX" );
      fEAX = 0;
    }
    word fSwap = 0;
    word pType;
    //{//select
    if ( Node[ Node[ P ].pRight ].ID == iCHAR || Node[ Node[ P ].pRight ].ID == iWORD || Node[ Node[ P ].pRight ].ID == iINT ) {
      pType = Code( Node[ P ].pLeft, 0, 0, 0 );
      fSwap = 2;
    }
    else if ( Node[ Node[ P ].pLeft ].ID == iCHAR || Node[ Node[ P ].pLeft ].ID == iWORD || Node[ Node[ P ].pLeft ].ID == iINT ) {
      Code( Node[ P ].pRight, 0, 0, 0 );
      fEAX = 1;
      pType = Code( Node[ P ].pLeft, 0, 0, 0 );
      fSwap = 1;
    }
    else if ( Node[ Node[ Node[ P ].pLeft ].pLeft ].ID == iLOAD ) {
      Code( Node[ P ].pRight, 0, 0, 0 );
      fEAX = 1;
      pType = Code( Node[ P ].pLeft, 0, 0, 0 );
      fSwap = 1;
    }
    else { //default
      pType = Code( Node[ P ].pLeft, 0, 0, 0 );
      fEAX = 1;
      Code( Node[ P ].pRight, 0, 0, 0 );
      if ( fEAX == 0 ) {
        Emit( 0, "pop     EBX" );
        fSwap = 1;
      }
    }
    word ID = Node[ P ].ID;
    //{//select
    if ( fSwap == 1 ) {
      //{//select
      if ( ID == iLT ) {
        ID = iGT;
      }
      else if ( ID == iLE ) {
        ID = iGE;
      }
      else if ( ID == iGE ) {
        ID = iLE;
      }
      else if ( ID == iGT ) {
        ID = iLT;
      }
      else if ( ID == iSUB || ID == iDIV || ID == iMOD ) {
        Emit( 0, "xchg    EAX,  EBX" );
      }
    }
    else if ( fSwap == 2 ) {
      //{//select
      if ( ID == iMUL || ID == iDIV || ID == iMOD ) {
        fEAX = 1;
        Code( Node[ P ].pRight, 0, 0, 0 );
      }
    }
    fEAX = 0;
    word Size = 4;
    if ( pType < nDICT ) {
      Size = Dict[ pType ].Value;
    }
    //{//select
    if ( iLT <= ID && ID <= iGT ) {
      //{//select
      if ( Size == 1 ) {
        //{//select
        if ( fSwap == 2 ) {
          Emit( 0, strcat( strcpy( Buff, "cmp     AL,   " ), str( Node[ Node[ P ].pRight ].Value ) ) );
        }
        else { //default
          Emit( 0, "cmp     AL,   BL" );
        }
      }
      else if ( Size == 4 ) {
        //{//select
        if ( fSwap == 2 ) {
          Emit( 0, strcat( strcpy( Buff, "cmp     EAX,  " ), str( Node[ Node[ P ].pRight ].Value ) ) );
        }
        else { //default
          Emit( 0, "cmp     EAX,  EBX" );
        }
      }
      else { //default
        Stop( eINTERNAL );
      }
      if ( T == 0 ) {
        //{//select
        if ( ID == iLT ) {
          ID = iGE;
        }
        else if ( ID == iLE ) {
          ID = iGT;
        }
        else if ( ID == iEQ ) {
          ID = iNE;
        }
        else if ( ID == iNE ) {
          ID = iEQ;
        }
        else if ( ID == iGE ) {
          ID = iLT;
        }
        else if ( ID == iGT ) {
          ID = iLE;
        }
        else { //default
          Stop( eINTERNAL );
        }
        T = F;
        F = 0;
      }
      if ( T == 0 || F != 0 ) {
        Stop( eINTERNAL );
      }
      //{//select
      if ( pType == Find( "int" ) ) {
        //{//select
        if ( ID == iLT ) {
          Emit( 0, strcat( strcpy( Buff, "jl      @" ), str( T ) ) );
        }
        else if ( ID == iLE ) {
          Emit( 0, strcat( strcpy( Buff, "jle     @" ), str( T ) ) );
        }
        else if ( ID == iEQ ) {
          Emit( 0, strcat( strcpy( Buff, "je      @" ), str( T ) ) );
        }
        else if ( ID == iNE ) {
          Emit( 0, strcat( strcpy( Buff, "jne     @" ), str( T ) ) );
        }
        else if ( ID == iGE ) {
          Emit( 0, strcat( strcpy( Buff, "jge     @" ), str( T ) ) );
        }
        else if ( ID == iGT ) {
          Emit( 0, strcat( strcpy( Buff, "jg      @" ), str( T ) ) );
        }
        else { //default
          Stop( eINTERNAL );
        }
      }
      else { //default
        //{//select
        if ( ID == iLT ) {
          Emit( 0, strcat( strcpy( Buff, "jb      @" ), str( T ) ) );
        }
        else if ( ID == iLE ) {
          Emit( 0, strcat( strcpy( Buff, "jbe     @" ), str( T ) ) );
        }
        else if ( ID == iEQ ) {
          Emit( 0, strcat( strcpy( Buff, "je      @" ), str( T ) ) );
        }
        else if ( ID == iNE ) {
          Emit( 0, strcat( strcpy( Buff, "jne     @" ), str( T ) ) );
        }
        else if ( ID == iGE ) {
          Emit( 0, strcat( strcpy( Buff, "jae     @" ), str( T ) ) );
        }
        else if ( ID == iGT ) {
          Emit( 0, strcat( strcpy( Buff, "ja      @" ), str( T ) ) );
        }
        else { //default
          Stop( eINTERNAL );
        }
      }
      return nDICT;
    }
    else if ( ID == iBIT_OR ) {
      strcpy( Buff, "or      " );
    }
    else if ( ID == iBIT_XOR ) {
      strcpy( Buff, "xor     " );
    }
    else if ( ID == iBIT_AND ) {
      strcpy( Buff, "and     " );
    }
    else if ( ID == iADD ) {
      strcpy( Buff, "add     " );
    }
    else if ( ID == iSUB ) {
      strcpy( Buff, "sub     " );
    }
    else if ( ID == iMUL ) {
      //{//select
      if ( pType == Find( "byte" ) ) {
        Emit( 0, "mul     BL" );
      }
      else if ( pType == Find( "word" ) ) {
        Emit( 0, "mul     EBX" );
      }
      else if ( pType == Find( "int" ) ) {
        Emit( 0, "imul    EBX" );
      }
      else { //default
        Stop( eINTERNAL );
      }
      return pType;
    }
    else if ( ID == iDIV ) {
      //{//select
      if ( pType == Find( "byte" ) ) {
        Emit( 0, "xor     AH,   AH" );
        Emit( 0, "div     BL" );
      }
      else if ( pType == Find( "word" ) ) {
        Emit( 0, "xor     EDX,  EDX" );
        Emit( 0, "div     EBX" );
      }
      else if ( pType == Find( "int" ) ) {
        Emit( 0, "xor     EDX,  EDX" );
        Emit( 0, "idiv    EBX" );
      }
      else { //default
        Stop( eINTERNAL );
      }
      return pType;
    }
    else if ( ID == iMOD ) {
      //{//select
      if ( pType == Find( "byte" ) ) {
        Emit( 0, "xor     AH,   AH" );
        Emit( 0, "div     BL" );
        Emit( 0, "xchg    AL,   AH" );
      }
      else if ( pType == Find( "word" ) ) {
        Emit( 0, "xor     EDX,  EDX" );
        Emit( 0, "div     EBX" );
        Emit( 0, "xchg    EAX,  EDX" );
      }
      else { //default
        Stop( eINTERNAL );
      }
      return pType;
    }
    else { //default
      Stop( eINTERNAL );
    }
    //{//select
    if ( Size == 1 ) {
      //{//select
      if ( fSwap == 2 ) {
        Emit( 0, strcat( strcat( Buff, "AL,   " ), str( Node[ Node[ P ].pRight ].Value ) ) );
      }
      else { //default
        Emit( 0, strcat( Buff, "AL,   BL" ) );
      }
    }
    else if ( Size == 4 ) {
      //{//select
      if ( fSwap == 2 ) {
        Emit( 0, strcat( strcat( Buff, "EAX,  " ), str( Node[ Node[ P ].pRight ].Value ) ) );
      }
      else { //default
        Emit( 0, strcat( Buff, "EAX,  EBX" ) );
      }
    }
    else { //default
      Stop( eINTERNAL );
    }
    return pType;
  }
  else if ( Node[ P ].ID == iDEF ) {
    if ( nParm >= nPARM ) {
      Stop( eNOMEMORY );
    }
    word pType = Node[ P ].Value;
    word nPtr = Node[ Node[ P ].pLeft ].Value;
    word Size = 4;
    if ( nPtr == 0 ) {
      Size = Dict[ Node[ P ].Value ].Value;
    }
    pVar = pVar + Even( Node[ Node[ Node[ P ].pLeft ].pLeft ].Value * Size );
    Parm[ nParm ].pType = pType;
    Parm[ nParm ].nPtr = nPtr;
    Parm[ nParm ].Value = pVar;
    ++ nParm;
    Code( Node[ Node[ P ].pLeft ].pLeft, 0, 0, 0 );
  }
  else if ( Node[ P ].ID == iPUSH ) {
    if ( Node[ P ].pRight < nNODE ) {
      Code( Node[ P ].pRight, 0, 0, 0 );
    }
    fEAX = 0;
    Code( Node[ P ].pLeft, 0, 0, 0 );
    Emit( 0, "push    EAX" );
  }
  else if ( Node[ P ].ID == iASSIGN ) {
    P = Node[ P ].pLeft;
    fEAX = 0;
    if ( Node[ P ].pRight < nNODE ) {
      Code( Node[ P ].pRight, 0, 0, 0 );
      fEAX = 1;
    }
    Code( Node[ P ].pLeft, 0, 0, 0 );
  }
  else if ( Node[ P ].ID == iRETURN ) {
    if ( Node[ P ].pLeft < nNODE ) {
      fEAX = 0;
      Code( Node[ P ].pLeft, 0, 0, 0 );
    }
    Emit( 0, "mov     ESP,  EBP" );
    Emit( 0, "pop     EBP" );
    if ( Dict[ pFunc ].Sub == sMAIN ) {
      Emit( 0, "xor     EAX,  EAX" );
      Emit( 0, "push    EAX" );
      Emit( 0, "call    [ExitProcess]" );
    }
    else {
      Emit( 0, strcat( strcpy( Buff, "ret     " ), str( pSize ) ) );
    }
  }
  else if ( Node[ P ].ID == iSELECT ) {
    word E = Node[ P ].Value;
    P = Node[ P ].pLeft;
    while ( P < nNODE ) {
      if ( Node[ Node[ P ].pLeft ].pLeft < nNODE ) {
        F = Node[ P ].Value;
        T = Node[ Node[ P ].pLeft ].Value;
        Code( Node[ Node[ P ].pLeft ].pLeft, F, 0, T );
        Emit( T, "" );
      }
      word nParm1 = nParm;
      word pVar1 = pVar;
      word P1 = Node[ Node[ P ].pLeft ].pRight;
      while ( P1 < nNODE ) {
        Code( P1, 0, 0, 0 );
        P1 = Node[ P1 ].pRight;
      }
      pVar = pVar1;
      nParm = nParm1;
      if ( Node[ P ].pRight < nNODE ) {
        Emit( 0, strcat( strcpy( Buff, "jmp     @" ), str( E ) ) );
      }
      if ( Node[ Node[ P ].pLeft ].pLeft < nNODE ) {
        Emit( F, "" );
      }
      P = Node[ P ].pRight;
    }
    Emit( E, "" );
  }
  else if ( Node[ P ].ID == iWHILE ) {
    F = Node[ P ].Value;
    P = Node[ P ].pLeft;
    T = Node[ P ].Value;
    word L = Node[ Node[ P ].pLeft ].Value;
    Emit( L, "" );
    if ( Node[ Node[ P ].pLeft ].pLeft < nNODE ) {
      Code( Node[ Node[ P ].pLeft ].pLeft, F, 0, T );
      Emit( T, "" );
    }
    word nParm1 = nParm;
    word pVar1 = pVar;
    word P1 = Node[ P ].pRight;
    while ( P1 < nNODE ) {
      Code( P1, 0, 0, 0 );
      P1 = Node[ P1 ].pRight;
    }
    pVar = pVar1;
    nParm = nParm1;
    Emit( 0, strcat( strcpy( Buff, "jmp     @" ), str( L ) ) );
    Emit( F, "" );
  }
  else if ( Node[ P ].ID == iLOOP || Node[ P ].ID == iEXIT ) {
    Emit( 0, strcat( strcpy( Buff, "jmp     @" ), str( Node[ Node[ P ].Value ].Value ) ) );
  }
  else if ( Node[ P ].ID == iEMPTY ) {
    ; //null
  }
  else { //default
    Stop( eINTERNAL );
  }
  return nDICT;
}
void  main( int argc, char *arg[] )
{
  word I = 0;
  word K = 0;
  char Name [ nBUFF ];
  if ( argc < 2 )
  {
    puts("Тpанслятоp языка Context. Веpсия 2.0");
    puts("(C) Хохлов А.В.  г.Москва  1995-2003");
    puts("Использование:  context  <имя файла>");
    return ;
  }
  strcpy( Name, arg[ 1 ] );
  word pExt = 0;
  I = 0;
  K = 0;
  while ( Name[ I ] != 0 ) {
    //{//select
    if ( Name[ I ] == '.' ) {
      pExt = I;
    }
    else if ( Name[ I ] == '\\' ) {
      pExt = 0;
      K = I + 1;
    }
    ++ I;
  }
  if ( pExt == 0 ) {
    if ( I + 4 >= nBUFF ) {
      puts( "Слишком длинное имя файла\r\n" );
      return ;
    }
    strcpy( &Name[ I ], ".ctx" );
    pExt = I;
    I = I + 4;
  }
  if ( I - K >= nINFO ) {
    puts( "Слишком длинное имя файла\r\n" );
    return ;
  }
  strcpy( Info, &Name[ K ] );
  hText = open( Name );
  if ( hText == 0xFFFFFFFF ) {
    puts( strcat( strcpy( Name, Info ), "(0): Невозможно откpыть файл\r\n" ) );
    return ;
  }
  nDict = 0;
  nParm = 0;
  nIndx = 0;
  nChar = 0;
  nNode = 0;
  nFile = 0;
  pFile = 0;
  strcpy( Text, "define @COMPILER \"Context 2.0 for Win32\"\n" );
  strcat( Text, "void ExitProcess( word uExitCode ) external KERNEL32;" );
  nText = strlen( Text );
  pText = 0;
  nLine = 0;
  pLoop = nNODE;
  Word( "include" );
  Word( "define" );
  Word( "struct" );
  Word( "synonym" );
  Word( "function" );
  Word( "external" );
  Word( "begin" );
  Word( "select" );
  Word( "case" );
  Word( "default" );
  Word( "if" );
  Word( "then" );
  Word( "else" );
  Word( "while" );
  Word( "do" );
  Word( "loop" );
  Word( "exit" );
  Word( "inc" );
  Word( "dec" );
  Word( "return" );
  Word( "null" );
  Word( "NULL" );
  Word( "end" );
  Base( "void", 0 );
  Base( "char", 1 );
  Base( "byte", 1 );
  Base( "word", 4 );
  Base( "int",  4 );
  char Buff [ nBUFF ];
  do {
    Scan( Buff );
    //{//select
    if ( strcmp( Buff, "begin" ) == 0 ) {
      if ( nDict >= nDICT ) {
        Stop( eNOMEMORY );
      }
      pFunc = nDict;
      pParm = nParm;
      pIndx = nIndx;
      Dict[ nDict ]. Class = cFUNC; strcpy( Dict[ nDict ].Name, "" );
      Dict[ nDict ]. Sub = sMAIN;
      Dict[ nDict ].pType = Find( "void" );
      Dict[ nDict ].nPtr = 0;
      Dict[ nDict ].pIndx = nParm;
      Dict[ nDict ].nIndx = nParm;
      Dict[ nDict ]. Flag = 1;
      ++ nDict;
      word *P = &Dict[ pFunc ].pNode;
      word Flag = 0;
      Scan( Buff );
      while ( Flag == 0 || strcmp( Buff, "end" ) != 0 ) {
        *P = Ctrl( Buff );
        P = &Node[ *P ].pRight;
        Flag = 1;
      }
      nIndx = pIndx;
      nParm = pParm;
      break; //exit
    }
    else if ( strcmp( Buff, "include" ) == 0 ) {
      if ( strcmp( Scan( Buff ), "\"" ) != 0 ) {
        Stop( eQUOTEXP );
      }
      word P = 0;
      while ( Read() != '"' ) {
        if ( Read() == '\n' || Read() == '\r' || Read() == '\t' || Read() == ' ' ) {
          Stop( eQUOTEXP );
        }
        Buff[ P ] = Read();
        ++ P;
        if ( P >= nNAME ) {
          Stop( eLONGNAME );
        }
        Next();
      }
      Buff[ P ] = 0;
      Next();
      P = Find( Buff );
      if ( P < nDict ) {
        if ( Dict[ P ].Class != cFILE ) {
          Stop( eDUPLICATE );
        }
        continue; //loop
      }
      if ( nDict >= nDICT ) {
        Stop( eNOMEMORY );
      }
      Dict[ nDict ].Class = cFILE; strcpy( Dict[ nDict ].Name, Buff );
      Dict[ nDict ].Flag = 0;
      ++ nDict;
      if ( nFile >= nFILE ) {
        Stop( eNOMEMORY );
      }
      word hFile = open( Buff );
      if ( hFile == 0xFFFFFFFF ) {
        Stop( eNOFILE );
      }
      char Temp[ nBUFF ];
      puts ( strcat( strcat( strcat( strcat( strcpy( Temp, "\r" ), Info ), "(" ), str( nLine ) ), ")\r\n" ) );
      File[ nFile ].hText = hText; strcpy( File[ nFile ].Info, Info );
      File[ nFile ].nText = nText;
      File[ nFile ].pText = pText;
      File[ nFile ].nLine = nLine;
      ++ nFile;
      I = 0;
      K = 0;
      while ( Buff[ I ] != 0 ) {
        if ( Buff[ I ] == '\\' ) {
          K = I + 1;
        }
        ++ I;
      }
      pFile = pFile + nTEXT;
      hText = hFile; strcpy( Info, &Buff[ K ] );
      nText = pFile;
      pText = pFile;
      nLine = 1;
    }
    else if ( strcmp( Buff, "define" ) == 0 ) {
      word nPtr = 0;
      if ( strcmp( Scan( Buff ), "@" ) == 0 ) {
        Scan( Buff );
        nPtr = 1;
      }
      if ( Find( Buff ) < nDict ) {
        Stop( eDUPLICATE );
      }
      if ( nDict >= nDICT ) {
        Stop( eNOMEMORY );
      }
      Dict[ nDict ]. Class = cCONST; Copy( Dict[ nDict ].Name, Buff, nDict );
      Dict[ nDict ].nPtr = 0;
      Dict[ nDict ]. Flag = 0;
      Scan( Buff );
      //{//select
      if ( nPtr == 0 ) {
        //{//select
        if ( strcmp( Buff, "'" ) == 0 ) {
          Dict[ nDict ].pType = Find( "char" );
          Dict[ nDict ].Value = word( Sign() );
          if ( Read() != '\'' ) {
            Stop( eQUOTEXP );
          }
          Next();
        }
        else if ( strcmp( Buff, "#" ) == 0 ) {
          word N = val( Scan( Buff ) );
          if ( N > 255 ) {
            Stop( eOVERFLOW );
          }
          Dict[ nDict ].pType = Find( "char" );
          Dict[ nDict ].Value = N;
        }
        else { //default
          Dict[ nDict ].pType = Find( "word" );
          Dict[ nDict ].Value = val ( Buff );
        }
      }
      else { //default
        if ( strcmp( Buff, "\"" ) != 0 ) {
          Stop( eQUOTEXP );
        }
        Line();
      }
      ++ nDict;
    }
    else if ( strcmp( Buff, "struct" ) == 0 ) {
      word pType = Find( Scan( Buff ) );
      if ( pType < nDict ) {
        Stop( eDUPLICATE );
      }
      if ( nDict >= nDICT ) {
        Stop( eNOMEMORY );
      }
      Dict[ nDict ]. Class = cTYPE; Copy( Dict[ nDict ].Name, Buff, nDict );
      Dict[ nDict ]. Sub = sSTRUCT;
      Dict[ nDict ].pIndx = nParm;
      if ( strcmp( Scan( Buff ), ";" ) == 0 ) {
        Stop( eNOTAVAIL );
      }
      word pParm = nParm;
      do {
        pType = Find( Buff );
        if ( pType >= nDict || Dict[ pType ].Class != cTYPE ) {
          Stop( eTYPEEXP );
        }
        do {
          word nPtr = 0;
          while ( strcmp( Scan( Buff ), "@" ) == 0 ) {
            ++ nPtr;
          }
          if ( pType == Find( "void" ) && nPtr < 1 ) {
            Stop( eNOVOID );
          }
          I = pParm;
          while ( I < nParm ) {
            if ( strcmp( Parm[ I ].Name, Buff ) == 0 ) {
              Stop( eDUPLICATE );
            }
            ++ I;
          }
          if ( nParm >= nPARM ) {
            Stop( eNOMEMORY );
          }
          Parm[ nParm ].pType = pType; Copy( Parm[ nParm ].Name, Buff, 0 );
          Parm[ nParm ].nPtr = nPtr;
          Parm[ nParm ].pIndx = nIndx;
          Parm[ nParm ].nIndx = Tabl( Scan( Buff ) );
          ++ nParm;
          if ( strcmp( Buff, ";" ) == 0 ) {
            break; //exit
          }
          if ( strcmp( Buff, "," ) != 0 ) {
            Stop( eSEMICOLONEXP );
          }
        }
        while ( 1 );
        if ( strcmp( Scan( Buff ), "end" ) == 0 ) {
          break; //exit
        }
      }
      while ( 1 );
      Dict[ nDict ].nIndx = nParm;
      Dict[ nDict ]. Flag = 0;
      ++ nDict;
    }
    else if ( strcmp( Buff, "synonym" ) == 0 ) {
      if ( Find( Scan( Buff ) ) < nDict ) {
        Stop( eDUPLICATE );
      }
      if ( nDict >= nDICT ) {
        Stop( eNOMEMORY );
      }
      Dict[ nDict ].Class = cNAME; Copy( Dict[ nDict ].Name, Buff, nDict );
      word pType = Find( Scan( Buff ) );
      if ( pType >= nDict ) {
        Stop( eUNDEFINED );
      }
      Dict[ nDict ].pType = pType;
      Dict[ nDict ]. Flag = 0;
      ++ nDict;
    }
    else { //default
      word pType = Find( Buff );
      if ( pType >= nDict || Dict[ pType ].Class != cTYPE ) {
        Stop( eTYPEEXP );
      }
      if ( strcmp( Scan( Buff ), "function" ) == 0 ) {
        Stop( eNOTAVAIL );
      }
      word Flag = 0;
      do {
        word nPtr = 0;
        while ( strcmp( Buff, "@" ) == 0 ) {
          ++ nPtr;
          Scan( Buff );
        }
        if ( Find( Buff ) < nDict ) {
          Stop( eDUPLICATE );
        }
        if ( nDict >= nDICT ) {
          Stop( eNOMEMORY );
        }
        Dict[ nDict ].pType = pType; Copy( Dict[ nDict ].Name, Buff, nDict );
        Dict[ nDict ].nPtr = nPtr;
        Dict[ nDict ]. Flag = 0;
        Scan( Buff );
        //{//select
        if ( strcmp( Buff, "(" ) == 0 ) {
          if ( Flag != 0 ) {
            Stop( eNOFUNCTION );
          }
          pFunc = nDict;
          Dict[ nDict ].Class = cFUNC;
          Dict[ nDict ].Sub = sFUNC;
          Dict[ nDict ].pIndx = nParm;
          Dict[ nDict ].nIndx = List( Scan( Buff ) );
          ++ nDict;
          Scan( Buff );
          //{//select
          if ( strcmp( Buff, ";" ) == 0 ) {
            Stop( eNOTAVAIL );
          }
          else if ( strcmp( Buff, "external" ) == 0 ) {
            word pLib = Find( Scan( Buff ) );
            if ( pLib >= nDict ) {
              if ( nDict >= nDICT ) {
                Stop( eNOMEMORY );
              }
              Dict[ nDict ]. Class = cFUNC; strcpy( Dict[ nDict ].Name, Dict[ pFunc ].Name );
              Dict[ nDict ].pType = pType;
              Dict[ nDict ].nPtr = nPtr;
              Dict[ nDict ].pIndx = Dict[ pFunc ].pIndx;
              Dict[ nDict ].nIndx = Dict[ pFunc ].nIndx;
              Dict[ nDict ]. Flag = 0;
              Dict[ pFunc ]. Class = cLIB; Copy ( Dict[ pFunc ].Name, Buff, nDict );
              pLib = pFunc;
              pFunc = nDict;
              ++ nDict;
            }
            if ( strcmp( Scan( Buff ), "." ) == 0 ) {
              if ( Find( Scan( Buff ) ) < nDict ) {
                Stop( eDUPLICATE );
              }
              if ( nDict >= nDICT ) {
                Stop( eNOMEMORY );
              }
              Dict[ nDict ]. Class = cNAME; strcpy( Dict[ nDict ].Name, Dict[ pFunc ].Name );
              Dict[ nDict ].pType = pFunc;
              Dict[ nDict ]. Flag = 0;
              ++ nDict;
              Copy( Dict[ pFunc ].Name, Buff, nDict );
              Scan( Buff );
            }
            Dict[ pFunc ].Sub = sSYS;
            Dict[ pFunc ].pNode = pLib;
            if ( strcmp( Buff, ";" ) != 0 ) {
              Stop( eSEMICOLONEXP );
            }
          }
          else { //default
            pParm = nParm;
            pIndx = nIndx;
            Flag = 0;
            word *P = &Dict[ pFunc ].pNode;
            while ( Flag == 0 || strcmp( Buff, "end" ) != 0 ) {
              *P = Ctrl( Buff );
              P = &Node[ *P ].pRight;
              Flag = 1;
            }
            nIndx = pIndx;
            nParm = pParm;
          }
          break; //exit
        }
        else { //default
          if ( pType == Find( "void" ) && nPtr < 1 ) {
            Stop( eNOVOID );
          }
          Dict[ nDict ]. Class = cDATA;
          Dict[ nDict ].pIndx = nIndx;
          Dict[ nDict ].nIndx = Tabl( Buff );
          ++ nDict;
          if ( strcmp( Buff, ";" ) == 0 ) {
            break; //exit
          }
          if ( strcmp( Buff, "," ) != 0 ) {
            Stop( eSEMICOLONEXP );
          }
          Scan( Buff );
        }
        Flag = 1;
      }
      while ( 1 );
    }
  }
  while ( 1 );
  close ( hText );
  while ( nFile > 0 ) {
    -- nFile;
    close( File[ nFile ].hText );
  }
  Mark( Dict[ pFunc ].pNode );
  Dict[ Find( "COMPILER" ) ] .Flag = 1;
  Dict[ Find( "ExitProcess" ) ].Flag = 1;
  strcpy( &Name[ pExt ], ".asm" );
  hText = create( Name );
  nText = 0;
//  Decl( ".386" );
//  Decl( ".model  flat,   stdcall" );

  Decl( "format PE GUI" );
  Decl( "entry start" );

  I = 0;
/*  while ( I < nDict ) {
    if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub == sSYS && Dict[ I ].Flag != 0 ) {
      word N = strlen( strcat( strcpy( Buff, "extrn   " ), Dict[ I ].Name ) );
      while ( N < nNAME + 8 ) {
        Buff[ N ] = ' ';
        ++ N;
      }
      Buff[ N ] = 0;
      Emit( 0, strcat( Buff, ":proc" ) );
    }
    ++ I;
  }*/
  word T = 0;
  I = 0;
  while ( I < nDict ) {
    if ( Dict[ I ].Class == cCONST && Dict[ I ].nPtr > 0 && Dict[ I ].Flag != 0 ) {
      Dict[ I ].Value = T;
      T = T + ( Dict[ I ].nIndx - Dict[ I ].pIndx );
    }
    ++ I;
  }
  Decl( "" );
//  Decl( ".code" );
  Decl( "section '.code' code readable executable" );
  word L = 10000;
  word D = 0;
  word S;
  word pRet = Peek();
  Node[ pRet ].ID = iRETURN;
  I = 0;
  while ( I < nDict ) {
    //{//select
    if ( Dict[ I ].Class == cTYPE && Dict[ I ].Sub == sSTRUCT ) {
      word D1 = 0;
      K = Dict[ I ].pIndx;
      while ( K < Dict[ I ].nIndx ) {
        Parm[ K ].Value = D1;
        word D2 = 4;
        if ( Parm[ K ].nPtr == 0 ) {
          D2 = Dict[ Parm[ K ].pType ].Value;
        }
        word N = Parm[ K ].pIndx;
        while ( N < Parm[ K ].nIndx ) {
          D2 = Indx[ N ] * D2;
          ++ N;
        }
        D1 = D1 + D2;
        ++ K;
      }
      Dict[ I ].Value = D1;
    }
    else if ( Dict[ I ].Class == cDATA && Dict[ I ].Flag != 0 ) {
      Dict[ I ].Value = D;
      word Size = 4;
      if ( Dict[ I ].nPtr == 0 ) {
        Size = Dict[ Dict[ I ].pType ].Value;
      }
      K = Dict[ I ].pIndx;
      while ( K < Dict[ I ].nIndx ) {
        Size = Indx[ K ] * Size;
        ++ K;
      }
      D = D + Even( Size );
    }
    else if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub != sSYS && Dict[ I ].Flag != 0 ) {
      Decl( "" );
      if ( Dict[ I ].Sub != sMAIN ) {
        Emit( L, strcat( strcpy( Buff, "       ;" ), Dict[ I ].Name ) );
      }
      else {
        Emit( L, "\nstart:       ;begin" );
        S = L;
      }
      Dict[ I ].Value = L;
      ++ L;
      word S1 = 0;
      K = Dict[ I ].pIndx;
      while ( K < Dict[ I ].nIndx ) {
        Parm[ K ].Value = S1 + 8;
        if ( Parm[ K ].nPtr == 0 ) {
          S1 = S1 + Even( Dict[ Parm[ K ].pType ].Value );
        }
        else {
          S1 = S1 + 4;
        }
        ++ K;
      }
      Emit( 0, "push    EBP" );
      Emit( 0, "mov     EBP,  ESP" );
      word V = Enum( Dict[ I ].pNode, 1, L );
      if ( V > 0 ) {
        Emit( 0, strcat( strcpy( Buff, "sub     ESP,  " ), str( V ) ) );
      }
      pParm = nParm;
      pFunc = I;
      pSize = S1;
      pVar = 0;
      word P = Dict[ I ].pNode;
      word ID;
      while ( P < nNODE ) {
        Code ( P, 0, 0, 0 );
        ID = Node[ P ].ID;
        P = Node[ P ].pRight;
      }
      if ( ID != iRETURN ) {
        Code( pRet, 0, 0, 0 );
      }
      nParm = pParm;
    }
    ++ I;
  }
  Decl( "" );
//  Decl( ".data" );
  Decl( "section '.data' data readable writeable" );
  strcpy( Buff, "\n@@TEXT  db     " );
  I = 0;
  while ( I < nDict ) {
    if ( Dict[ I ].Class == cCONST && Dict[ I ].nPtr > 0 && Dict[ I ].Flag != 0 ) {
      word N = strlen( Buff );
      word Flag = 0;
      K = Dict[ I ].pIndx;
      while ( K < Dict[ I ].nIndx ) {
        if ( N + 4 >= nBUFF ) {
          Stop( eINTERNAL );
        }
        if ( Char[ K ] == '\n' || Char[ K ] == '\r' || Char[ K ] == '\t' || Char[ K ] == '\"' || Char[ K ] == '\0' ) {
          if ( Flag != 0 ) {
            Buff[ N ] = '"';
            ++ N;
            Flag = 0;
          }
          if ( K > Dict[ I ].pIndx ) {
            Buff[ N ] = ',';
            ++ N;
          }
          //{//select
          if ( Char[ K ] == '\n' ) {
            Buff[ N ] = '1';
            ++ N;
            Buff[ N ] = '0';
            ++ N;
          }
          else if ( Char[ K ] == '\r' ) {
            Buff[ N ] = '1';
            ++ N;
            Buff[ N ] = '3';
            ++ N;
          }
          else if ( Char[ K ] == '\t' ) {
            Buff[ N ] = '9';
            ++ N;
          }
          else if ( Char[ K ] == '\"' ) {
            Buff[ N ] = '3';
            ++ N;
            Buff[ N ] = '4';
            ++ N;
          }
          else if ( Char[ K ] == '\0' ) {
            Buff[ N ] = '0';
            ++ N;
          }
        }
        else {
          if ( Flag == 0 ) {
            if ( K > Dict[ I ].pIndx ) {
              Buff[ N ] = ',';
              ++ N;
            }
            Buff[ N ] = '"';
            ++ N;
            Flag = 1;
          }
          Buff[ N ] = Char[ K ];
          ++ N;
        }
        ++ K;
      }
      Buff[ N ] = 0;
      Decl ( Buff );
      strcpy( Buff, "        db     " );
    }
    ++ I;
  }
  if ( D > 0 ) {
    Decl( "" );
    Decl( strcat( strcat( strcpy( Buff, "@@DATA  rb      " ), str( D ) ), "" ) );
  }
  Decl( "" );
//  Emit( 0, strcat( strcpy( Buff, "end     @" ), str( S ) ) );
//  if ( nText > 0 ) {
//    write( hText, Text, nText );
//  }
//  close ( hText );
//
//  strcpy( &Name[ pExt ], ".def" );
//  hText = create( Name );
//  nText = 0;
  I = 0;

  Decl( "section '.idata' import data readable writeable" );
  Decl( "" );

  int   flag  = 0;
  char  temp[ nBUFF ];

  while ( I < nDict ) {
    if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub == sSYS && Dict[ I ].Flag != 0 ) {
      if( flag == 0 ) {
        Decl( strcat( strcat( strcat( strcat( strcpy( Buff, "dd\t0,0,0,RVA\t" ), Dict[ Dict[ I ].pNode ].Name ), "_name,\tRVA\t" ), Dict[ Dict[ I ].pNode ].Name ), "_table" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
        flag  = 1;
      }
      else if ( strcmp( temp, Dict[ Dict[ I ].pNode ].Name ) != 0 ) {
        Decl( strcat( strcat( strcat( strcat( strcpy( Buff, "dd\t0,0,0,RVA\t" ), Dict[ Dict[ I ].pNode ].Name ), "_name,\tRVA\t" ), Dict[ Dict[ I ].pNode ].Name ), "_table" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
      }
    }
    ++ I;
  }
  Decl( "dd\t0,0,0,0,0" );
  Decl( "" );

  I = 0;
  flag  = 0;

  while ( I < nDict ) {
    if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub == sSYS && Dict[ I ].Flag != 0 ) {
      if( flag == 0 ) {
        Decl( strcat( strcpy( Buff, Dict[ Dict[ I ].pNode ].Name ), "_table:" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
        flag  = 1;
      }
      else if ( strcmp( temp, Dict[ Dict[ I ].pNode ].Name ) != 0 ) {
        Decl( "\tdd\t0" );
        Decl( "" );
        Decl( strcat( strcpy( Buff, Dict[ Dict[ I ].pNode ].Name ), "_table:" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
      }
      Decl( strcat( strcat( strcat( strcpy( Buff, "\t" ), Dict[ I ].Name ), "\tdd\tRVA\t_" ), Dict[ I ].Name ) );
    }
    ++ I;
  }
  Decl( "\tdd\t0" );
  Decl( "" );

  I = 0;
  flag  = 0;

  while ( I < nDict ) {
    if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub == sSYS && Dict[ I ].Flag != 0 ) {
      if( flag == 0 ) {
        Decl( strcat( strcat( strcat( strcpy( Buff, Dict[ Dict[ I ].pNode ].Name ), "_name\tdb\t'" ), Dict[ Dict[ I ].pNode ].Name ), ".DLL',0" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
        flag  = 1;
      }
      else if ( strcmp( temp, Dict[ Dict[ I ].pNode ].Name ) != 0 ) {
        Decl( strcat( strcat( strcat( strcpy( Buff, Dict[ Dict[ I ].pNode ].Name ), "_name\tdb\t'" ), Dict[ Dict[ I ].pNode ].Name ), ".DLL',0" ) );
        strcpy( temp, Dict[ Dict[ I ].pNode ].Name );
      }
    }
    ++ I;
  }
  Decl( "" );

  I = 0;

  while ( I < nDict ) {
    if ( Dict[ I ].Class == cFUNC && Dict[ I ].Sub == sSYS && Dict[ I ].Flag != 0 ) {
      Decl( strcat( strcat( strcpy( Buff, "_" ), Dict[ I ].Name ), "\tdw\t0" ) );
      Decl( strcat( strcat( strcpy( Buff, "\tdb\t'" ), Dict[ I ].Name ), "',0" ) );
    }
    ++ I;
  }

  Decl( "" );
  Decl( "section '.reloc' fixups data readable discardable" );

  puts ( strcat( strcat( strcat( strcat( strcpy( Buff, "\r" ), Info ), "(" ), str( nLine ) ), ")\r\n" ) );

  if ( nText > 0 ) {
    write( hText, Text, nText );
  }
  close( hText );
}
