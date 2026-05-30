procedure main is
begin
end main;

function testa (ativo : Boolean; x : Integer) return Boolean is
  ok : Boolean;
begin
  ok := not False = (x < 10);
  return ok;
end testa;
