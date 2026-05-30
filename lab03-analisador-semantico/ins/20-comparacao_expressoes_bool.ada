procedure main is
begin
end main;

function testar (a : Integer; b : Integer; c : Integer) return Boolean is
  resultado : Boolean;
begin
  resultado := (a < b) and (b < c);
  return resultado;
end testar;
