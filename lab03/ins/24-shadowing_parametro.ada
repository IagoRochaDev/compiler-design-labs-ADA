procedure main is
begin
end main;

function sombra_parametro (X : Integer) return Integer is
   Acum : Integer;
begin
   Acum := X * 2;
   
   declare
      X : Integer;
      Resultado : Integer;
   begin
      X := Acum + 9;
      Resultado := X * X;
      return Resultado;
   end;
   
end sombra_parametro;