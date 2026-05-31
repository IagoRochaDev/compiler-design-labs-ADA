procedure main is
begin
end main;

function calcular (x : Float) return Float is
  a : Float;
  b : Float;
  c : Float;
begin
  a := x * 2.0;
  b := a + 1.0;
  c := b / 2.0;
  return c;
end calcular;
