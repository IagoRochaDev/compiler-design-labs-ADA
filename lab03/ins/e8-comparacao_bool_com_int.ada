procedure main is
begin
end main;

function erroMenorBool(ativo : Boolean; limite : Integer) return Boolean is
  resultado : Boolean;
begin
  resultado := ativo < limite;
  return resultado;
end erroMenorBool;
