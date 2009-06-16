int      strlen(char @St)
  int    P=0;
  while  St[P]!=#0 do
    inc  P;
  end

  return P;
end


int      strcmp(char @St1, @St2)
  int    P=0;
  while  St1[P]=St2[P] do
    if St1[P]=#0 then
      return 0;
    end

    inc P;
  end

  if St1[P]<St2[P] then
    return  -1;
  end

  return     1;
end


char    @strcpy(char @Dst, @Src)
  int    P=0;
  while  Src[P]!=#0 do
    Dst [P]=Src[P];
    inc  P;
  end
  Dst[P]=#0;

  return @Dst;
end


char    @strcat(char @Dst, @Src)
  strcpy(@Dst[strlen(@Dst)],@Src);
  return @Dst;
end


char    @str(int N)
  char  @Buff ="-0000000000"; // 10 знаков (max -2**31=-2147483648)
  char  @Digit= "0123456789";

  word   S=0;
  if N<0 then
    Buff[0]='-';

    N=-N;
    S= 1;
  end

  word   W=N;
  word   I=S;
  while  W>0 | I=S do
    Buff[I]=Digit[W%10];
    inc  I;

    W=W/10;
  end
  Buff[I]=#0;

  word   J=S;
  word   K=I-1;
  while  J<K do
    char C =Buff[J];
    Buff[J]=Buff[K];
    Buff[K]=C;

    dec  K;
    inc  J;
  end

  return @Buff;
end
