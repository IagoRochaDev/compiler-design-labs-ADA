procedure main is
begin
end main;

function erroIgualdadeTiposMistos(x : Integer; b : Boolean) return Boolean is
  resultado : Boolean;
begin
  resultado := x = b;
  return resultado;
end erroIgualdadeTiposMistos;
