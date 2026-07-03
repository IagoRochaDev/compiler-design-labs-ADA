procedure main is
begin
end main;

function erroSomaBool(b : Boolean; x : Integer) return Integer is
  resultado : Integer;
begin
  resultado := b + x;
  return resultado;
end erroSomaBool;
