procedure main is
begin
end main;

function media (a : Integer; b : Integer; c : Integer) return Integer is
  soma : Integer;
  resultado : Integer;
begin
  soma := a + b + c;
  resultado := soma / 3;
  return resultado;
end media;
