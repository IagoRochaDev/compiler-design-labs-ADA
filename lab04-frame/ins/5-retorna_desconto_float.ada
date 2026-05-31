procedure main is
begin
end main;

function descontar (preco : Float; taxa : Float) return Float is
  final : Float;
begin
  final := preco * (1.0 - taxa);
  return final;
end descontar;
