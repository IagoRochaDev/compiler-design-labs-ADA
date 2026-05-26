with Ada.Text_IO; use Ada.Text_IO;

procedure compil_lab1_amostra_b_iago_rocha is

   type Ponto_T is record
      X : Integer;
      Y : Integer;
   end record;

   type Triangulo_T is record
      A, B, C : Ponto_T;
      Cor     : Integer;
   end record;

   type Vetor_Pontos is array (Integer range <>) of Ponto_T;

   function Func (V : Vetor_Pontos; N : Integer; T : Triangulo_T) return Float is
      Res  : Float;
      Temp : Float;

      Rec_Val : Float; 
   begin
      if N <= 0 then
         return 1.0;
      elsif N = 1 then
         return 1.01 + Float(V(V'First).X) / 1.0e2 + Float(V(V'First).Y) / 0.1e-2 
                - Float(T.A.X * T.A.X) + Float(T.B.Y * T.C.X);
      end if;

      Res := 0.25e-13;

      for I in reverse 0 .. N - 1 loop
         
         exit when V(I).X <= 0; 

         Temp := Float((V(I).Y * V(I).X) mod 123);

         if Temp < 0.0 then
             Res := Res - (Res * 2.0e-2) + Func(V, N - 1, T) * Temp - Float(T.A.Y * T.Cor);
         else
             Res := Res + (Res * 0.3e3) + Func(V, N - 2, T) * Temp + Float(T.C.X * T.Cor);
             Put_Line("Estranho, ne?");
         end if;
      end loop;

      return Res;
   end Func;

   function F2 (T : Triangulo_T) return Integer is
      A     : Integer := 0;
      Soma  : array (0 .. 9) of Float;
      Total : Integer := 0;
   begin
      if (T.A.X >= 10 or else T.B.Y > 20 or else T.A.Y < 30 or else T.B.X <= 50) 
         and then not (T.C.X /= 90 or else T.C.Y = 0) 
      then
         return 10 mod 3;
      else
         A := 1;
      end if;

      while A < 10 loop
         Total := 0;
         Total := Total + (T.C.X * T.C.Y);
         Total := Total + (T.B.X * T.A.Y);
         Total := Total + (T.A.X * T.B.Y);
         
         Soma(A) := Float(Total mod 100);
         A := A + 1;
      end loop;
      return 0; 
   end F2;


begin
   null;
end compil_lab1_amostra_b_iago_rocha;