procedure main is
  x : Integer;
  resultado : Integer;
  y : Integer;
begin
  x := 5;
  
  resultado := x + 1;
  
  y := dobro(x);
end main;

function dobro(x : Integer) return Integer is
  sucesso : Integer;
begin
  sucesso := verificar(x + 1);

  return x;
end dobro;