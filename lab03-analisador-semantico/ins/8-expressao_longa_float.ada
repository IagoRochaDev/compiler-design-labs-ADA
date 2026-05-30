procedure main is
begin
end main;

function calcular (a : Float; b : Float; c : Float) return Float is
  resultado : Float;
begin
  resultado := a * b + c / 2.0 - a;
  return resultado;
end calcular;
